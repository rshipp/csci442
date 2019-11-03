require_relative '../result'


describe Result do
  let(:return_code) { 0 }
  let(:success_prompt) { "joshthomas :) > " }

  let(:input) do
    [ "echo 1 2 3 4",
      "ls /this/does/not/exist",
      "pwd",
      "alias",
      "ls"
    ]
  end

  let(:output) do
    [ "joshthomas :) > echo 1 2 3 4",
      "1 2 3 4 ",
      "joshthomas :) > ls /this/does/not/exist",
      "ls error: No such file or directory",
      "joshthomas :( > pwd",
      "/Users/joshthomas/Desktop/csci442/projects/1/jthomas-impl",
      "joshthomas :) > alias",
      "joshthomas :) > ls",
      ".git",
      ".gitignore",
      "command_t.h",
      "main.cpp",
      "joshthomas :) > "
    ]
  end

  let(:result) do
    Result.new.tap do |r|
      r.input = input
      r.output = output
      r.return_code = return_code
      r.success_prompt = success_prompt
    end
  end

  describe '[]' do
    subject { result[cmd] }

    context 'simple single line output' do
      let (:cmd) { 0 }

      it { should == output[1] }
    end

    context 'error command' do
      let (:cmd) { 1 }

      it { should == output[3] }
    end

    context 'output contains username' do
      let (:cmd) { 2 }

      it { should == output[5] }
    end

    context 'empty output' do
      let (:cmd) { 3 }

      it { should == '' }
    end

    context 'multiline' do
      let (:cmd) { 4 }

      it { should == output[8..11].join("\n") }
    end
  end
end
