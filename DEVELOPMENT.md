# For fellow developers out there who want to help

## Source changes

- Code changes **MUST** be submitted to `development` branch.
- Other changes, i.e. `readme` can be submitted directly to main branch.

## Code style
```c
// Naming convention
typedef struct StructName {
    size_t all_integers_must_be_size_t_or_ssize_t;
} StructsAreCamelCaseWithFirstLetterCapitalized;

void functionsAreCamelCase(void)
{
    char *variables_are_snake_case = 1;
}

// Spacing
a += 1
x = y*z - 1 + w/2
if(a == b){...}

// Curly braces
// Loops must ALWAYS use curly
for(int i=0; i<10;i++){
    printf("Hi :)\n");
}
// Functions
void foo(size_t a)
{
    printf("bar\n");
}

// Comments start from capitalized letter and have SPACE before start
// If logial code block ends, it must be written like this
// Drawing Statistics   // is start of code block
//-Drawing-Statistics.- // is end of code block
```
