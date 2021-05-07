#include "path.h"
#include "env.h"

char	*g_cwd;

/* g_cwd に新しいパスをセットする.
 * chdir() などはしない.
 */
int	set_current_working_directory(char *abs_path)
{
	char	*oldpwd;

	oldpwd = get_env_val("PWD");
	free(g_cwd);
	g_cwd = ft_strdup(abs_path);
	ft_setenv("OLDPWD", oldpwd, 1);
	ft_setenv("PWD", abs_path, 1);
	free(oldpwd);
	return (0);
}

/* 絶対パスを構築する
 * "{g_cwd}/{relative_path}" を返す
 * ex:
 *   - relative_path="dir/dir2/symlink2dir"
 *   - relative_path="./../dir/../././/.//////./"
 */
char *get_abs_path_from_cwd(char *relative_path)
{
	char	**dirs;  // ("..", ".", その他) みたいなのが入ってくる
	char	*result;
	char	*tmp;
	int		i;

	if (!g_cwd)
		g_cwd = getcwd(NULL, 0);
	result = ft_strdup(g_cwd);
	dirs = ft_split(relative_path, '/');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		tmp = result;
		result = path_join(tmp, dirs[i]);
		free(tmp);
		i++;
	}
	free_ptrarr((void **)dirs);
	tmp = result;
	result = canonicalize_path(tmp);
	free(tmp);
	return (result);
}

/* パスを正規化する.
 * ex:
 *   - "////hoge//fuga//../././//gaga" -> "/hoge/gaga"
 */
char	*canonicalize_path(char *path)
{
	char	**dirs;
	char	*result;
	char	*tmp;
	int		i;

	dirs = ft_split(path, '/');
	i = 0;
	result = ft_strdup("/");
	while (dirs[i])
	{
		if (ft_strncmp(dirs[i], "..", 3) == 0)
		{
			// "/" 以外の時, 親ディレクトリに移動する
			if (ft_strncmp(result, "/", 2) && ft_strlen(result))
			{
				tmp = result;
				result = ft_substr(tmp, 0, ft_strrchr(tmp, '/') - tmp);
				free(tmp);
			}
		}
		else if (ft_strncmp(dirs[i], ".", 2) == 0)
			;
		else
		{
			tmp = result;
			result = path_join(tmp, dirs[i]);
			free(tmp);
		}
		i++;
	}
	free_ptrarr((void **)dirs);
	return (result);
}

/* 親ディレクトリに移動した後のパスを返す
 */
char	*change_to_parent_dir(char *path)
{
	char	*result;

	result = ft_substr(path, 0, ft_strrchr(path, '/') - path);
	if (result && ft_strlen(result) == 0)
	{
		free(result);
		result = ft_strdup("/");
	}
	return (result);
}

/*
 * concatenate dirpath, "/" and filename.
 */
char	*path_join(char *dirpath, char *filename)
{
	char	*tmp;
	char	*result;

	if (ft_strlen(dirpath) == 0 || dirpath[ft_strlen(dirpath) - 1] != '/')
		tmp = ft_strjoin(dirpath, "/");
	else
		tmp = ft_strdup(dirpath);
	if (!tmp)
		return (NULL);
	result = ft_strjoin(tmp, filename);
	free(tmp);
	return (result);
}
