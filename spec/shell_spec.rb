require 'pathname'
require 'tempfile'
require_relative 'spec_helper'
require_relative '../shell_runner'

unless File.directory?(ENV['SHELL_DIR'].to_s)
  raise 'SHELL_DIR is not a directory'
end


shared_context 'shell context' do
  let(:commands) { [] }
  let!(:result) { ShellRunner.instance.run(commands) }
end


shared_examples 'successful command' do
  it "should succeed (return code of 0)" do
    result.should be_success
  end
end


shared_examples 'failed command' do
  it "should fail (non-zero return code)" do
    result.should_not be_success
  end
end


describe 'The prompt', d1: true do
  include_context 'shell context'

  let(:final_prompt) { result.result_prompt }

  context 'username' do
    it 'is correct' do
      final_prompt.should include(ENV['USER'])
    end
  end

  context 'on failure' do
    let(:commands) { ['ls /does/not/exist/42'] }

    it 'indicates failure' do
      result.result_prompt.should_not == result.success_prompt
    end
  end

  context 'on success' do
    let(:commands) { ['ls /does/not/exist/42', 'echo 1'] }

    it 'indicates success' do
      result.result_prompt.should == result.success_prompt
    end
  end
end


describe 'pwd', d1: true do
  include_context 'shell context'

  let(:commands) { ['pwd'] }

  include_examples 'successful command'

  it 'should print current working directory' do
    result[0].should == Dir.getwd
  end

  context 'with extra args' do
    let (:commands) { ['pwd extra args'] }

    include_examples 'failed command'
  end
end


describe 'ls', d1: true do
  include_context 'shell context'

  context 'with no arguments' do
    let(:commands) { ['ls'] }

    include_examples 'successful command'

    it 'should output the files in the current directory' do
      `/bin/ls`.split(/\s+/).each do |file|
        result[0].should include(file)
      end
    end
  end

  context 'with one arg' do
    let(:commands) { ["ls #{dir}"] }

    context 'and the directory exists' do
      let(:dir) { '/tmp' }

      include_examples 'successful command'

      it 'should output the files in that directory' do
        `/bin/ls #{dir}`.split(/\s+/).each do |file|
          result[0].should include(file)
        end
      end
    end

    context 'and the directory does not exist' do
      let(:dir) { '/bad/no/exist/32' }

      include_examples 'failed command'
    end
  end

  context 'with multiple args' do
    let(:commands) { ["ls /tmp /bad/no/exist/83"] }

    include_examples 'failed command'
  end
end


describe 'cd', d1: true do
  include_context 'shell context'

  context 'with no args' do
    let(:commands) { ['cd', 'pwd', 'cd'] }

    include_examples 'successful command'

    it "should change to user's home directory" do
      result[1].should include(ENV['HOME'])
    end
  end

  context 'with one arg' do
    let(:commands) { ["cd #{dir}", "pwd", "cd #{dir}"] }

    context 'and the directory exists' do
      let(:dir) { '/tmp' }

      include_examples 'successful command'

      it "should set CWD to the directory" do
        result[1].should include(dir)
      end
    end

    context 'and the directory does not exist' do
      let(:dir) { '/bad/no/exist/32' }

      include_examples 'failed command'
    end
  end

  context 'with multiple args' do
    let(:commands) { ["cd /tmp /bad/no/exist/83"] }

    include_examples 'failed command'
  end
end


describe 'history', d1: true do
  include_context 'shell context'

  context 'with no args' do
    let(:commands) { ['echo super unique', '', 'echo 2 3 4', 'history'] }
    let(:non_empty_cmds) { commands[0..-2].reject(&:empty?) }

    include_examples 'successful command'

    it 'should output 1-indexed history (excluding empty commands)' do
      first_command_idx = command_idx = result[3][/(\d+).*#{commands[0]}/, 1]

      first_command_idx.should_not be_nil
      first_command_idx.should_not == '0'

      non_empty_cmds.each do |cmd, i|
        result[3].should match(/#{command_idx}.*#{cmd}/)
        command_idx.succ!
      end
    end

    it 'should include the history command, as well' do
      result[3].should include('history')
    end
  end

  context 'with extra args' do
    let(:commands) { ['history extra args'] }

    include_examples 'failed command'
  end
end


describe 'echo', d1: true do
  include_context 'shell context'

  context 'with no args' do
    let(:commands) { ['echo'] }

    include_examples 'successful command'

    it 'should output an empty line' do
      result[0].should == ''
    end
  end

  context 'with args' do
    let(:commands) { ['echo one 2 three 4 five'] }

    include_examples 'successful command'

    it "should output args" do
      result[0].should include(commands[0][5..-1])
    end
  end
end


describe 'exit', d1: true do
  include_context 'shell context'

  let(:commands) { ['exit', 'echo 1 2 3'] }

  include_examples 'successful command'

  it "should exit immediately" do
    expect{ result[1] }.to raise_error(ArgumentError)
  end
end


describe 'alias', d1: true do
  include_context 'shell context'

  context 'with no args' do
    let(:commands) { ['alias key=value', 'alias mix=match', 'alias'] }

    include_examples 'successful command'

    it 'should print existing aliases' do
      result[2].should match(/key.*value/)
      result[2].should match(/mix.*match/)
    end
  end

  context 'with one arg' do

    context 'with format name=value' do
      let(:commands) { ["alias print=echo"] }

      include_examples 'successful command'

      it 'should create a functioning alias' do
        cmds = commands + ['print 1 2 3']
        ShellRunner.instance.run(cmds)[1].should include('1 2 3')
      end
    end

    context 'with invalid format' do
      let(:commands) { ['alias print'] }

      include_examples 'failed command'
    end
  end

  context 'with more than 1 arg' do
    let(:commands) { ['alias this=really bad'] }

    include_examples 'failed command'
  end
end


describe 'unalias', d1: true do
  include_context 'shell context'

  context 'with no args' do
    let(:commands) { ['unalias'] }

    include_examples 'failed command'
  end

  context 'with one arg' do

    context '-a' do
      let(:commands) { ['alias print=echo', 'unalias -a'] }

      include_examples 'successful command'

      it 'should unset all aliases' do
        cmds = commands + ['alias']
        ShellRunner.instance.run(cmds)[2].should_not include('print')
      end
    end

    context 'equal to an existing alias' do
      let(:commands) { ['alias print=echo', 'unalias print'] }

      include_examples 'successful command'

      it 'should unset that alias' do
        cmds = commands + ['alias']
        ShellRunner.instance.run(cmds)[2].should_not include('print')
      end
    end

  end

  context 'with more than 1 arg' do
    let(:commands) { ['unalias this is invalid'] }

    include_examples 'failed command'
  end
end


describe 'variable assignment', d2: true do
  include_context 'shell context'

  let(:commands) { ['X42=42 Y10=10 echo $Y10 $X42'] }

  it 'create local variables' do
    result[0].should include('10 42')
  end
end


describe 'variable completion', d2: true do
  include_context 'shell context'

  context 'for environment variables' do
    let(:commands) { ["echo $P\t\t "] }
    let(:matches) { ENV.keys.select {|v| v[0] == 'P' }.map {|v| "$#{v}" } }

    it 'should include all matches' do
      matches.each {|match| result.output.join.should include(match) }
    end
  end

  context 'for local variables' do
    let(:commands) { ["CATBEAR=a CAT_VIDEO=b", "echo $CAT\t\t "] }
    let(:matches) { ['$CATBEAR', '$CAT_VIDEO'] }

    it 'should include all matches' do
      matches.each {|match| result.output.join.should include(match) }
    end
  end
end


describe 'command completion', d2: true do
  include_context 'shell context'

  context 'for builtins' do
    let(:commands) { ["unal\t\t"] }

    it 'should include all matches' do
      result.output.join.should include('unalias')
    end
  end

  context 'for aliases' do
    let(:commands) { ["alias haikyuu=echo", "haik\t\t 1 2 3"] }

    it 'should include all matches' do
      result.output[1..-1].join.should include('haikyuu')
    end
  end

  context 'for external commands' do
    let(:commands) { ["gunz\t\t"] }

    it 'should include all matches' do
      result.output.join.should include('gunzip')
    end
  end
end


describe 'filename completion', d2: true do
  include_context 'shell context'

  let(:commands) { ["/bin/ls -a /usr/include/readline/\t\t"] }

  it 'should include all matches' do
    Dir.entries('/usr/include/readline').each do |file|
      result.output.join.should include(file)
    end
  end
end


describe 'history expansion', d2: true do
  include_context 'shell context'

  let(:commands) do
    ["echo it is work or anime ", "echo !!:$ !!:^-2"]
  end

  it 'should work using the readline history library' do
    result.output.join.should include("anime it is")
  end
end


describe 'external command execution', df: true do
  include_context 'shell context'

  let(:outfile) { Tempfile.new('st').path }

  context 'for a single command', d2: true do
    let(:commands) { ['hostname'] }

    include_examples 'successful command'

    it 'should execute the command' do
      result[0].should == `hostname`.chomp
    end

    context 'that fails' do
      let(:commands) { ['cat /does/not/exist'] }

      include_examples 'failed command'
    end
  end

  context 'writing output to a file via >' do
    let(:commands) { ["#{command} > #{outfile}"] }
    let(:command) { 'cat /etc/hosts' }

    include_examples 'successful command'

    it "should write the command's output to the file" do
      File.open(outfile).read.should == `#{command}`
    end

    context 'when the file cannot be written' do
      let(:outfile) { '/so/sad/does/not/work' }

      include_examples 'failed command'
    end

    context 'when the command fails' do
      let(:commands) { ["cat 42 > #{outfile}"] }

      include_examples 'failed command'
    end
  end

  context 'appending output to a file via >>' do
    let(:commands) { ["#{command} >> #{outfile}", "#{command} >> #{outfile}"] }
    let(:command) { 'cat /etc/hosts' }

    include_examples 'successful command'

    it "should append the command's output to the file" do
      File.open(outfile).read.should == `#{command}` * 2
    end

    context 'when the file cannot be written' do
      let(:outfile) { '/so/sad/does/not/work' }

      include_examples 'failed command'
    end

    context 'when the command fails' do
      let(:commands) { ["cat 42 >> #{outfile}"] }

      include_examples 'failed command'
    end
  end

  context 'reading input from a file via <' do
    let(:commands) { ['cat < /etc/hosts'] }

    include_examples 'successful command'

    it 'should execute the command using the file as input' do
      result[0].should == File.open('/etc/hosts').read.chomp
    end

    context "that doesn't exist" do
      let(:commands) { ['cat < /no/such/file/61'] }

      include_examples 'failed command'
    end

    context 'when the command fails' do
      let(:commands) { ['cat -X < /etc/hosts'] }

      include_examples 'failed command'
    end
  end

  context 'with a single pipe' do
    let(:commands) { ['cat /etc/hosts | grep localhost'] }

    include_examples 'successful command'

    it 'should use the output of the first command as input to the second' do
      result[0].should == `#{commands[0]}`.chomp
    end

    context 'where the first command fails but the second succeeds' do
      let(:commands) { ['cat /failure/is/assured | /bin/echo'] }

      include_examples 'successful command'
    end

    context 'where the final command fails' do
      let(:commands) { ['cat /etc/hosts | grep -M misuse'] }

      include_examples 'failed command'
    end
  end

  context 'with several arguments and a pipe' do
    let(:commands) { ['/bin/echo -n this is a test | wc -l'] }

    include_examples 'successful command'

    it 'should successfully execute both commands' do
      result[0].should == `#{commands[0]}`.chomp
    end
  end

  context 'with multiple pipes' do
    let(:commands) { ['cat /etc/hosts | grep 127 | sort | wc -l'] }

    include_examples 'successful command'

    it 'should successfully execute all commands' do
      result[0].should == `#{commands[0]}`.chomp
    end
  end

  context 'with a pipe and > file redirection' do
    let(:commands) { ["#{command} > #{outfile}"] }
    let(:command) { 'cat /etc/hosts | wc' }

    include_examples 'successful command'

    it 'should write the output of the pipe chain to the file' do
      File.open(outfile).read.should == `#{command}`
    end
  end

  context 'with a pipe and >> file redirection' do
    let(:commands) { ["#{command} >> #{outfile}", "#{command} >> #{outfile}"] }
    let(:command) { 'cat /etc/hosts | wc' }

    include_examples 'successful command'

    it 'should append the output of the pipe chain to the file' do
      File.open(outfile).read.should == `#{command}` * 2
    end
  end

  context 'with a pipe and < file redirection' do
    let(:commands) { ["grep localhost < /etc/hosts | wc"] }

    include_examples 'successful command'

    it 'should use the file as input to the pipe chain' do
      result[0].should == `#{commands[0]}`.chomp
    end
  end

  context 'with missing file argument' do

    context 'for >' do
      let(:commands) { ['date >'] }

      include_examples 'failed command'
    end

    context 'for >>' do
      let(:commands) { ['date >>'] }

      include_examples 'failed command'
    end

    context 'for <' do
      let(:commands) { ['grep <'] }

      include_examples 'failed command'
    end

    context 'for < before pipe' do
      let(:commands) { ['grep < |'] }

      include_examples 'failed command'
    end
  end

  context 'with empty commands' do

    context 'before >' do
      let(:commands) { ['> out.txt'] }

      include_examples 'failed command'
    end

    context 'before >>' do
      let(:commands) { ['>> out.txt'] }

      include_examples 'failed command'
    end

    context 'before <' do
      let(:commands) { ['< in.txt'] }

      include_examples 'failed command'
    end

    context 'before |' do
      let(:commands) { ['| wc'] }

      include_examples 'failed command'
    end

    context 'after |' do
      let(:commands) { ['date |'] }

      include_examples 'failed command'
    end

    context 'between pipes' do
      let(:commands) { ['date | | wc'] }

      include_examples 'failed command'
    end
  end

  context 'with multiple inputs' do

    context 'via 2 input files' do
      let(:commands) { ['grep text < 1.txt < 2.txt'] }

      include_examples 'failed command'
    end

    context 'via input file and pipe' do
      let(:commands) { ['cat 1.txt | grep text < 2.txt'] }

      include_examples 'failed command'
    end
  end

  context 'with multiple outputs' do

    context 'via 2 output files' do
      let(:commands) { ['date > 1.txt > 2.txt'] }

      include_examples 'failed command'
    end

    context 'via output file and pipe' do
      let(:commands) { ['date > out.txt | wc'] }

      include_examples 'failed command'
    end
  end
end
