#include "test.h"
#include "../parse.h"
#include "../execution.h"
#include "../minishell.h"

void	init_buf_with_string(t_parse_buffer *buf, const char* str)
{
	buf->cur_pos = 0;
	strcpy(buf->buffer, str);
	buf->size = strlen(str);
}

void check_strarr(const char **actual_strarr, const char **expected_strarr)
{
	int i = 0 ;
	while (actual_strarr[i] && expected_strarr[i])
	{
		CHECK_EQ_STR(actual_strarr[i], expected_strarr[i]);
		i++;
	}
	CHECK(!(actual_strarr || expected_strarr));  // 両方ともNULLだよね?
}

void check_cmdinvo(t_command_invocation *actual_cmdinvo, t_command_invocation *expected_invo)
{
	while (actual_cmdinvo && expected_invo)
	{
		if (actual_cmdinvo->input_file_path || expected_invo->input_file_path)
			CHECK_EQ_STR(actual_cmdinvo->input_file_path, expected_invo->input_file_path);
		if (actual_cmdinvo->output_file_path || expected_invo->output_file_path)
			CHECK_EQ_STR(actual_cmdinvo->output_file_path, expected_invo->output_file_path);
		check_strarr(actual_cmdinvo->exec_and_args, expected_invo->exec_and_args);
		actual_cmdinvo = actual_cmdinvo->piped_command;
		expected_invo = expected_invo->piped_command;
	}
	CHECK(!(actual_cmdinvo || expected_invo));  // 両方ともNULLだよね?
}

int main()
{
	TEST_SECTION("シンプルな文字列");
	{
		/* 準備 */
		t_parse_buffer	buf;
		init_buf_with_string(&buf, "abc \n");
		t_token	tok;

		lex_get_token(&buf, &tok);

		t_parse_ast *node = parse_piped_commands(&buf, &tok);
        CHECK_EQ(node->type, ASTNODE_PIPED_COMMANDS);

		/* テスト */
		t_command_invocation *actual = cmd_ast_pipcmds2cmdinvo(node->content.piped_commands);
		t_command_invocation *expected = cmd_init_cmdinvo(NULL, NULL, (const char **)ft_split("abc", ' '), 0);
		CHECK(actual);
		check_cmdinvo(actual, expected);
	}
}
