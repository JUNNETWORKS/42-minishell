#include "minishell.h"

/*
 * first に second を繋げて, firstだけfreeする
 */
static char	*strjoin_and_free_first(char *first, char *second)
{
	char	*tmp;

	tmp = first;
	first = ft_strjoin(first, second);
	free(tmp);
	return (first);
}

/* string_nodeを文字列に戻す
 *うまくいけばASTの時点でこの文字列になるように依頼する
 */
char	*string_node2string(t_parse_node_string *string_node)
{
	char	*result;

	result = ft_strdup("");
	while (string_node)
	{
		if (string_node->type == TOKTYPE_NON_EXPANDABLE)
			result = strjoin_and_free_first(result, "\'");
		else if (string_node->type == TOKTYPE_EXPANDABLE_QUOTED)
			result = strjoin_and_free_first(result, "\"");
		result = strjoin_and_free_first(result, string_node->text);
		if (string_node->type == TOKTYPE_NON_EXPANDABLE)
			result = strjoin_and_free_first(result, "\'");
		else if (string_node->type == TOKTYPE_EXPANDABLE_QUOTED)
			result = strjoin_and_free_first(result, "\"");
		if (string_node->next)
			string_node = string_node->next->content.string;
		else
			break ;
	}
	return (result);
}
