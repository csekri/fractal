//header for l-system

struct Rules{
    char ancestor;
    char *child;
};

struct System{
//we don't need more than 20 rules
    struct Rules rule[20];
    char *axiom;
    int gen;
};

bool isValid(int n, char **data);

struct System conversion(int n, char **data);

long long int length(const struct System sys, int n, int numIt);

char *iteration(char **data, int n);
