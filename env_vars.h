#ifndef ENV_VARS_H
#define ENV_VARS_H

#include <stdlib.h>
#include <stdbool.h>

#define MAX_LINE 1000

typedef struct EnvVars EnvVars;

EnvVars *create_env_vars();
void destroy_env_vars(EnvVars *env_vars);
bool replace_env_vars(char *line, EnvVars *env_vars);

const char *get_env_var(EnvVars *env_vars, const char *key);
void set_env_var(EnvVars *env_vars, const char *key, const char *value);
void unset_env_var(EnvVars *env_vars, const char *key);

#endif // ENV_VARS_H
