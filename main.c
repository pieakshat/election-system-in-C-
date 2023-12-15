#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

FILE *file;

struct candidate_info // required candidate info
{
    char name[30];
    int age;
    int aadhar_no;
    int votes;
};

struct voter_info // required voter info
{
    char name[30];
    int age;
    int voterid;
};

void get_candidate_info() // function to get the candidate's info from the user
{
    struct candidate_info candidate;
    file = fopen("candidates.csv", "a"); // create a csv file that stores all the candidates and their info

    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    printf("enter candidate name: ");
    scanf("%s", candidate.name);
    // fgets(candidate.name, sizeof(candidate.name), stdin); // wierd///////???????????
    // candidate.name[strcspn(candidate.name, "\n")] = '\0'; // remove trailing newline

    printf("enter candidate age: ");
    scanf("%d", &candidate.age);
    if (candidate.age <= 25)
    {
        printf("The candidate must have a minimum age of 25 to participate in the elections");
        exit(1);
    }

    printf("enter candidate's aadhar number: ");
    scanf("%d", &candidate.aadhar_no);

    fprintf(file, "\n%s,%d,%d,%d\n", candidate.name, candidate.age, candidate.aadhar_no, 0); // print the collected info in csv
    printf("\n|-----the candidate has been registered-----|");

    char s[10];
    printf("\ndo you want to continue?(yes/no) ");
    scanf("%s", s);
    if (strcmp(s, "yes") == 0)
    {
        fclose(file);
        get_candidate_info();
    }
    else
    {
        fclose(file);
    }
    fclose(file);
}

int generate_voterid() // generate the voterid number for each voter
{
    int a = rand();
    return a;
}

void get_voter_info() // function to register voters
{
    struct voter_info voter;
    file = fopen("voter.csv", "a");

    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    printf("enter voter's name: ");
    scanf("%s", voter.name);

    printf("enter voter's age: ");
    scanf("%d", &voter.age);
    if (voter.age <= 18)
    {
        printf("The voter must have a minimum age of 18 to vote");
        exit(1);
    }
    srand(time(NULL)); // srand seeds the random number with current time thus ensuring voter id no generated is always different
    voter.voterid = generate_voterid();
    printf("your voter id is: %d", voter.voterid);
    // wait(2000);
    printf("\n |-----Thank you for registering-----|");
    fprintf(file, "%s,%d,%d\n", voter.name, voter.age, voter.voterid);
    // wait(2000);
    char s[5];
    printf("\ndo you want to continue?(yes/no) ");
    scanf("%s", s);
    if (strcmp(s, "yes") == 0)
    {
        get_voter_info();
    }

    fclose(file);
}

void display_candidates() // function to display all the candidates not in order
{
    FILE *file;
    file = fopen("candidates.csv", "r");

    if (file == NULL)
    {
        printf("Error opening the file.\n");
        return;
    }

    printf("| %-20s |\n", "Candidates");
    printf("|----------------------|\n");

    char line[100];
    int count = 0;
    while (fgets(line, sizeof(line), file))
    {
        struct candidate_info candidate;
        sscanf(line, "%49[^,\n]", candidate.name);
        count++;
        printf("| %d %-18s |\n", count, candidate.name);
    }
}

int verify_voter()
{
    struct voter_info voter;
    FILE *voterfile = fopen("voter.csv", "r");
    int voterid;
    int flag = 0;
    printf("enter voter id: ");
    scanf("%d", &voterid);
    char line[100];
    while (fgets(line, sizeof(line), voterfile))
    {
        sscanf(line, "%*[^,],%*d,%d", &voter.voterid);
        if (voter.voterid == voterid)
        {
            flag = 1;
        }
    }
    return flag;
}

void vote()
{
    struct candidate_info candidate;
    char selected_candidate[30];
    int votes = 0;

    int verified = verify_voter();
    int flag = 0;
    if (verified == 0)
    {
        printf("voter has not been registered");
        exit(1);
    }
    else
    {
        display_candidates();
        printf("select the candidate: ");
        scanf("%s", selected_candidate);

        FILE *candidatesFile = fopen("candidates.csv", "r");
        FILE *tempFile = fopen("temp.csv", "w");

        if (candidatesFile == NULL || tempFile == NULL)
        {
            printf("Error opening the file.\n");
            return;
        }

        char line[100];
        while (fgets(line, sizeof(line), candidatesFile))
        {
            sscanf(line, "%29[^,],%d,%d,%d", candidate.name, &candidate.age, &candidate.aadhar_no, &candidate.votes);

            if (strcmp(selected_candidate, candidate.name) == 0)
            {
                flag = 1;                                            // to check if the cndidate name is correct in the terminal
                sscanf(line, "%*[^,],%*d,%*d,%d", &candidate.votes); //%*d reads the int but doesn't assign it to any variable, essentially skipping over it
                candidate.votes++;                                   // Increment the vote count for the selected candidate
                fprintf(tempFile, "%s,%d,%d,%d\n", candidate.name, candidate.age, candidate.aadhar_no, candidate.votes);
            }
            else
            {
                if (line[0] != '\n')
                    fprintf(tempFile, "%s", line);
            }
        }
        if (flag == 0)
        {
            printf("candidate not found\n");
            remove("temp.csv");
            vote(); // /////
        }
        else
        {

            fclose(candidatesFile);
            fclose(tempFile);

            remove("candidates.csv");
            rename("temp.csv", "candidates.csv");
            printf("|--------Thank you for voting-------|");
        }
    }
}

void swap(struct candidate_info *a, struct candidate_info *b)
{
    struct candidate_info temp = *a;
    *a = *b;
    *b = temp;
}

void bubbleSort(struct candidate_info candidates[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i; j++)
        {
            if (candidates[j].votes < candidates[j + 1].votes)
            {
                swap(&candidates[j], &candidates[j + 1]);
            }
        }
    }
}

void display_candidates_sorted(struct candidate_info candidates[], int n)
{
    printf("| %-20s | %-5s |\n", "candidates", "votes");
    printf("|----------------------|-------|\n");

    for (int i = 0; i < n; i++)
    {
        printf("| %-20s | %-5d |\n", candidates[i].name, candidates[i].votes);
    }
}

void sorting_candidates()
{
    FILE *file;
    file = fopen("candidates.csv", "r");

    if (file == NULL)
    {
        printf("error opening the file.\n");
        return;
    }

    struct candidate_info candidates[100]; // declare an array to hold candidate info
    int candidateCount = 0;

    // read candidates
    char line[100];
    while (fgets(line, sizeof(line), file))
    {
        sscanf(line, "%29[^,],%d,%d,%d", candidates[candidateCount].name, &candidates[candidateCount].age, &candidates[candidateCount].aadhar_no, &candidates[candidateCount].votes);
        candidateCount++;
    }

    fclose(file);
    bubbleSort(candidates, candidateCount);

    display_candidates_sorted(candidates, candidateCount);
}

int main()
{
    printf("|---WELCOME---|\n");
    int choice;
    printf("1. voter registration\n2. candidate registration\n3. display candidates\n4. Vote\n5. view result");
    printf("\n please select the function you want to perform: ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        get_voter_info();
        break;
    case 2:
        get_candidate_info();
        break;
    case 3:
        display_candidates();
        break;
    case 4:
        vote();
        break;
    case 5:
        sorting_candidates();
        break;

    default:
        printf("invalid choice");
        break;
    }

    // get_candidate_info();
    //  get_voter_info();
    //   display_candidates();
    //   vote();
    //   sorting_candidates();
}