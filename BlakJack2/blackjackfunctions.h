void fixdeck(int deck1[], int deck[]);
void levelup(int played[],int& level,int card,double probs[],int deck[]);
void leveldown(int& card,int played[],int deck[],int& level);
void printfinalprobs(double finalprobs[6]);
void printplayed(int played[], int level, int card);
void replacewith1(int played[], int level);
void replacewith11(int played[], int level);

int aceposition(int played[], int level);
int decksum(int deck[]);
int sum(int played[], int level);

double addedfinalprobs(double finalprobs[6]);
double newprobcalculate( int played[], int level, int deckr[], int numcards);
double prob(int card, int deck[]);

