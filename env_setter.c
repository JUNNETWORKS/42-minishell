#include "env.h"

/*
 * keyname, value を元に "key=value" の文字列を作成する
 */
static char	*generate_kvstr(const char *key, const char *value)
{
	char	*tmp;
	char	*kvstr;

	tmp = ft_strjoin(key, "=");
	kvstr = ft_strjoin(tmp, value);
	free(tmp);
	return (kvstr);
}

/* 既に環境変数が存在している場合
 *   rewrite=1 なら書き換える.
 *   rewrite=0 なら書き換えない
 *
 * name: 環境変数のキー名
 * value: 環境変数の値
 * rewrite: 環境変数が存在していた場合に置き換えるかどうか
 * has_updated: 環境変数の更新が行われたらtrueが入る.
 *
 * return: 正常なら0. それ以外なら-1.
 */
static int	update_env(const char *name,
	const char *value, int rewrite, bool *has_updated)
{
	extern char	**environ;
	int			idx;
	char		*kvstr;
	int			name_len;

	idx = 0;
	name_len = ft_strlen(name);
	while (!*has_updated && environ[idx])
	{
		if (!ft_strncmp(environ[idx], name, name_len)
			&& environ[idx][name_len] == '=')
		{
			*has_updated = true;
			if (!rewrite)
				return (0);
			kvstr = generate_kvstr(name, value);
			if (!kvstr)
				return (ERROR);
			if ((void *)environ[idx] < (void *)&idx)
				free((void *)environ[idx]);
			environ[idx] = kvstr;
		}
		idx++;
	}
	return (0);
}

/*
 * environ より+2大きい配列を確保して環境変数を追加する.
 *
 * name: 環境変数のキー名
 * value: 環境変数の値
 *
 * return: 正常なら0. それ以外なら-1.
 */
static int	expand_and_add_env(const char *name, const char *value)
{
	extern char	**environ;
	char		**new_environ;
	int			environ_len;

	environ_len = ptrarr_len((void **)environ);
	new_environ = ft_calloc(environ_len + 2, sizeof(char *));
	if (!new_environ)
		return (ERROR);
	ft_memcpy(new_environ, environ, sizeof(char *) * environ_len);
	new_environ[environ_len] = generate_kvstr(name, value);
	if (!new_environ[environ_len])
	{
		free_ptrarr((void **)new_environ);
		return (ERROR);
	}
	if ((void *)environ < (void *)&new_environ)
		free(environ);
	environ = new_environ;
	return (0);
}

/*
 * 標準ライブラリの setenv() と同じ動作をする
 *
 * name: 環境変数のキー名
 * value: 環境変数の値
 * rewrite: 環境変数が既に存在している場合に書き換えるかどうか
 *
 * return: 正常なら0. それ以外なら-1.
 */
int	ft_setenv(const char *name, const char *value, int rewrite)
{
	bool	has_updated;

	has_updated = false;
	if (update_env(name, value, rewrite, &has_updated) == ERROR)
		return (ERROR);
	if (!has_updated && expand_and_add_env(name, value) == ERROR)
		return (ERROR);
	return (0);
}

/*
 * name で指定された環境変数が存在する場合, それを削除する.
 * 標準ライブラリの unsetenv() と同じ動作をする.
 *
 * name: 環境変数のキー名
 *
 * return: 正常なら0. それ以外なら-1.
 */
int	ft_unsetenv(const char *name)
{
	extern char	**environ;
	int			idx;
	int			env_len;
	int			name_len;

	idx = 0;
	name_len = ft_strlen(name);
	env_len = ptrarr_len((void **)environ);
	while (environ[idx])
	{
		if (ft_strncmp(environ[idx], name, name_len) == 0
			&& environ[idx][name_len] == '=')
		{
			if ((void *)environ[idx] < (void *)&idx)
				free(environ[idx]);
			ft_memmove(environ + idx, environ + idx + 1,
				sizeof(char *) * (env_len - idx));
			return (0);
		}
		idx++;
	}
	return (0);
}
