class Result
  attr_accessor :input, :output, :return_code, :success_prompt

  def result_prompt
    output.empty? ? nil : output.last
  end

  def success?
    return_code == 0
  end

  def prompt_correct?
    if success?
      result_prompt == success_prompt
    else
      result_prompt != success_prompt
    end
  end

  def [](cmd_idx)
    command = input[cmd_idx]

    raise "Can't find #{cmd_idx}" if command.nil?

    output_idx = output.find_index do |line|
      line.start_with?(ENV['USER']) && line.end_with?(command)
    end

    raise ArgumentError.new("Can't find command in output") if output_idx.nil?

    output[output_idx+1..-1].
        take_while {|line| !line.start_with?(ENV['USER']) }.
        join("\n")
  end

  def to_s
    "<Result status: #{return_code}>"
  end

  alias :inspect :to_s
end
