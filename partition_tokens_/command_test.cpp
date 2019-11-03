/**
 * Unit tests for the partition_tokens function. If they're helpful, great. If
 * not, feel free to ignore them. =)
 */

#include "command.h"
#include "gtest/gtest.h"
#include <sstream>
#include <iterator>

using namespace std;

// Forward declarations.
vector<string> tokenize(string);
vector<command_t> partition_string(string);
void assert_commands(vector<command_t>, vector<command_t>);
void assert_invalid(string, string);
bool operator ==(const command_t&, const command_t&);


// UNIT TESTS. Feel free to add your own or change the existing ones.


TEST(partition_tokens, one_command) {
  vector<command_t> results = partition_string("git diff");
  vector<command_t> expected(1);

  expected[0].argv = tokenize("git diff");
  expected[0].input_type = READ_FROM_STDIN;
  expected[0].output_type = WRITE_TO_STDOUT;

  assert_commands(expected, results);
}


TEST(partition_tokens, one_command_outfile_truncate) {
  vector<command_t> results = partition_string("git diff > patch.txt");
  vector<command_t> expected(1);

  expected[0].argv = tokenize("git diff");
  expected[0].input_type = READ_FROM_STDIN;
  expected[0].output_type = WRITE_TO_FILE;
  expected[0].outfile = "patch.txt";

  assert_commands(expected, results);
}


TEST(partition_tokens, one_command_outfile_append) {
  vector<command_t> results = partition_string("git diff >> patch.txt");
  vector<command_t> expected(1);

  expected[0].argv = tokenize("git diff");
  expected[0].input_type = READ_FROM_STDIN;
  expected[0].output_type = APPEND_TO_FILE;
  expected[0].outfile = "patch.txt";

  assert_commands(expected, results);
}


TEST(partition_tokens, one_command_infile) {
  vector<command_t> results = partition_string("git apply < patch.txt");
  vector<command_t> expected(1);

  expected[0].argv = tokenize("git apply");
  expected[0].input_type = READ_FROM_FILE;
  expected[0].output_type = WRITE_TO_STDOUT;
  expected[0].infile = "patch.txt";

  assert_commands(expected, results);
}


TEST(partition_tokens, simple_pipe) {
  vector<command_t> results = partition_string("date | wc");
  vector<command_t> expected(2);

  expected[0].argv = tokenize("date");
  expected[0].input_type = READ_FROM_STDIN;
  expected[0].output_type = WRITE_TO_PIPE;

  expected[1].argv = tokenize("wc");
  expected[1].input_type = READ_FROM_PIPE;
  expected[1].output_type = WRITE_TO_STDOUT;

  assert_commands(expected, results);
}


TEST(partition_tokens, simple_pipe_several_args) {
  string line = "/bin/echo -n this is a test | wc -l";
  vector<command_t> results = partition_string(line);
  vector<command_t> expected(2);

  expected[0].argv = tokenize("/bin/echo -n this is a test");
  expected[0].input_type = READ_FROM_STDIN;
  expected[0].output_type = WRITE_TO_PIPE;

  expected[1].argv = tokenize("wc -l");
  expected[1].input_type = READ_FROM_PIPE;
  expected[1].output_type = WRITE_TO_STDOUT;

  assert_commands(expected, results);
}


TEST(partition_tokens, multi_pipe) {
  string line = "cat main.cpp | grep shell | sort | wc -l";
  vector<command_t> results = partition_string(line);
  vector<command_t> expected(4);

  expected[0].argv = tokenize("cat main.cpp");
  expected[0].input_type = READ_FROM_STDIN;
  expected[0].output_type = WRITE_TO_PIPE;

  expected[1].argv = tokenize("grep shell");
  expected[1].input_type = READ_FROM_PIPE;
  expected[1].output_type = WRITE_TO_PIPE;

  expected[2].argv = tokenize("sort");
  expected[2].input_type = READ_FROM_PIPE;
  expected[2].output_type = WRITE_TO_PIPE;

  expected[3].argv = tokenize("wc -l");
  expected[3].input_type = READ_FROM_PIPE;
  expected[3].output_type = WRITE_TO_STDOUT;

  assert_commands(expected, results);
}


TEST(partition_tokens, pipe_and_file_write) {
  vector<command_t> results = partition_string("date | wc > result.txt");
  vector<command_t> expected(2);

  expected[0].argv = tokenize("date");
  expected[0].input_type = READ_FROM_STDIN;
  expected[0].output_type = WRITE_TO_PIPE;

  expected[1].argv = tokenize("wc");
  expected[1].input_type = READ_FROM_PIPE;
  expected[1].output_type = WRITE_TO_FILE;
  expected[1].outfile = "result.txt";

  assert_commands(expected, results);
}


TEST(partition_tokens, pipe_and_file_append) {
  vector<command_t> results = partition_string("date | wc >> result.txt");
  vector<command_t> expected(2);

  expected[0].argv = tokenize("date");
  expected[0].input_type = READ_FROM_STDIN;
  expected[0].output_type = WRITE_TO_PIPE;

  expected[1].argv = tokenize("wc");
  expected[1].input_type = READ_FROM_PIPE;
  expected[1].output_type = APPEND_TO_FILE;
  expected[1].outfile = "result.txt";

  assert_commands(expected, results);
}


TEST(partition_tokens, pipe_and_file_input) {
  vector<command_t> results = partition_string("grep 2016 < in.txt | wc -l");
  vector<command_t> expected(2);

  expected[0].argv = tokenize("grep 2016");
  expected[0].input_type = READ_FROM_FILE;
  expected[0].output_type = WRITE_TO_PIPE;
  expected[0].infile = "in.txt";

  expected[1].argv = tokenize("wc -l");
  expected[1].input_type = READ_FROM_PIPE;
  expected[1].output_type = WRITE_TO_STDOUT;

  assert_commands(expected, results);
}


TEST(invalid_inputs, missing_file_truncate) {
  assert_invalid("date >", "missing file argument for '>'");
}


TEST(invalid_inputs, missing_file_append) {
  assert_invalid("date >>", "missing file argument for '>>'");
}


TEST(invalid_inputs, missing_file_input) {
  assert_invalid("grep <", "missing file argument for '<'");
}


TEST(invalid_inputs, missing_file_before_pipe) {
  assert_invalid("grep < | ", "missing file argument for '<'");
}


TEST(invalid_inputs, missing_command_file_truncate) {
  assert_invalid("> out.txt ", "missing command for '>'");
}


TEST(invalid_inputs, missing_command_file_append) {
  assert_invalid(">> out.txt ", "missing command for '>'");
}


TEST(invalid_inputs, missing_command_file_input) {
  assert_invalid("< in.txt ", "missing command for '<'");
}


TEST(invalid_inputs, pipe_terminal_pipe) {
  assert_invalid("date |", "should not allow empty command");
}


TEST(invalid_inputs, pipe_empty_command) {
  assert_invalid("date | | wc", "should not allow empty command");
}


TEST(invalid_inputs, leading_pipe) {
  assert_invalid("| wc", "should not allow empty command");
}


TEST(invalid_inputs, multiple_input_2_files) {
  assert_invalid("grep text < 1.txt < 2.txt", "command has multiple inputs");
}


TEST(invalid_inputs, multiple_outputs_2_files) {
  assert_invalid("date > 1.txt > 2.txt", "command has multiple outputs");
}


TEST(invalid_inputs, multiple_inputs_file_and_pipe) {
  assert_invalid(
      "cat 1.txt | grep text < 2.txt",
      "command has multiple inputs");
}


TEST(invalid_inputs, multiple_outputs_file_and_pipe) {
  assert_invalid(
      "date > out.txt | wc",
      "command has multiple outputs");
}


// HELPER FUNCTIONS. Feel free to ignore.


// Runs partition_tokens on the given string (after tokenizing it).
vector<command_t> partition_string(string text) {
  vector<command_t> results;

  EXPECT_TRUE(partition_tokens(tokenize(text), results))
      << "Paritioning should succeed for:\n  " << text;

  return results;
}


// Converts the given string to a vector of tokens.
vector<string> tokenize(string text) {
  stringstream ss(text);

  return vector<string>(
      istream_iterator<string>(ss),
      istream_iterator<string>());
}


// Asserts that the partitioned results contain exactly the expected commands.
void assert_commands(vector<command_t> expected, vector<command_t> actual) {
  ASSERT_EQ(expected.size(), actual.size());

  for (size_t i = 0; i < expected.size(); i++) {
    EXPECT_EQ(expected[i], actual[i]);
  }
}


// Asserts that the given text cannot be successfully partitioned.
void assert_invalid(string line, string reason) {
  vector<command_t> unused;

  testing::internal::CaptureStdout();
  testing::internal::CaptureStderr();

  bool success = partition_tokens(tokenize(line), unused);

  string stdout_text = testing::internal::GetCapturedStdout();
  string stderr_text = testing::internal::GetCapturedStderr();

  EXPECT_FALSE(success)
      << "Paritioning should fail for:"
      << "\n  line: " << line
      << "\n  why:  " << reason;

  EXPECT_FALSE(stdout_text.empty() && stderr_text.empty())
      << "Should print an appropriate error message";
}


// Overload == for command_t
bool operator ==(const command_t& lhs, const command_t& rhs) {
  return lhs.argv == rhs.argv
      && lhs.input_type == rhs.input_type
      && lhs.output_type == rhs.output_type
      && lhs.infile == rhs.infile
      && lhs.outfile == rhs.outfile;
}
