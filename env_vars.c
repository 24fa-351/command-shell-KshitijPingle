// Add a header later

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "env_vars.h"

struct EnvVars
{
    char **keys;
    char **values;
    size_t size;
    size_t capacity;
};

EnvVars *create_env_vars()
{
    EnvVars *env_vars = (EnvVars *)malloc(sizeof(EnvVars));
    env_vars->size = 0;
    env_vars->capacity = 10;
    env_vars->keys = (char **)malloc(env_vars->capacity * sizeof(char *));
    env_vars->values = (char **)malloc(env_vars->capacity * sizeof(char *));
    return env_vars;
}

void destroy_env_vars(EnvVars *env_vars)
{
    for (size_t i = 0; i < env_vars->size; ++i)
    {
        free(env_vars->keys[i]);
        free(env_vars->values[i]);
    }
    free(env_vars->keys);
    free(env_vars->values);
    free(env_vars);
}

// Search for env vars with '$' and replace them with their values
void replace_env_vars(char *line, EnvVars *env_vars)
{
    char buffer[MAX_LINE];
    char *start = line;
    char *end;
    buffer[0] = '\0';

    while ((start = strchr(start, '$')) != NULL)
    {

        // Copy the part of the line before the $
        strncat(buffer, line, start - line);

        end = start + 1;

        // Find the end of the env var name
        while (*end && (isalnum(*end) || *end == '_'))
        {
            // Keeps looping until it finds a character that is not a letter, number, or underscore
            end++;
        }
        char var_name[100];
        strncpy(var_name, start + 1, end - start - 1);
        var_name[end - start - 1] = '\0';
        const char *var_value = get_env_var(env_vars, var_name);
        if (var_value)
        {
            strcat(buffer, var_value);
        }
        line = end;
        start = end;
    }
    strcat(buffer, line);
    strcpy(line, buffer);

    return;
}

const char *get_env_var(EnvVars *env_vars, const char *key)
{
    for (size_t i = 0; i < env_vars->size; ++i)
    {
        if (strcmp(env_vars->keys[i], key) == 0)
        {
            return env_vars->values[i];
        }
    }
    return NULL;
}

void set_env_var(EnvVars *env_vars, const char *key, const char *value)
{

    // Check if the key already exists
    for (size_t i = 0; i < env_vars->size; ++i)
    {
        if (strcmp(env_vars->keys[i], key) == 0)
        {
            free(env_vars->values[i]);
            env_vars->values[i] = strdup(value);
            return;
        }
    }

    // Add the key-value pair
    if (env_vars->size == env_vars->capacity)
    {
        env_vars->capacity *= 2;
        env_vars->keys = (char **)realloc(env_vars->keys, env_vars->capacity * sizeof(char *));
        env_vars->values = (char **)realloc(env_vars->values, env_vars->capacity * sizeof(char *));
    }

    env_vars->keys[env_vars->size] = strdup(key);
    env_vars->values[env_vars->size] = strdup(value);
    env_vars->size++;
}

void unset_env_var(EnvVars *env_vars, const char *key)
{
    for (size_t i = 0; i < env_vars->size; ++i)
    {
        if (strcmp(env_vars->keys[i], key) == 0)
        {
            free(env_vars->keys[i]);
            free(env_vars->values[i]);
            env_vars->keys[i] = env_vars->keys[env_vars->size - 1];
            env_vars->values[i] = env_vars->values[env_vars->size - 1];
            env_vars->size--;
            return;
        }
    }
}
