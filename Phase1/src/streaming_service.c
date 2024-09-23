#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "streaming_service.h"

void print_enum(int num)
{
    switch (num)
    {
    case 0:
        printf("HORROR");
        break;
    case 1:
        printf("SCIFI");
        break;
    case 2:
        printf("DRAMA");
        break;
    case 3:
        printf("ROMANCE");
        break;
    case 4:
        printf("DOCUMENTARY");
        break;
    case 5:
        printf("COMEDY");
        break;
    default:
        printf("Movie category did not found \n");
        break;
    }
}

/*------WatchHistory POP-------------------*/
struct movie *pop(struct movie **watchHistory)
{
    if (*watchHistory == NULL)
    {
        return NULL;
    }
    struct movie *to_return = (*watchHistory);
    (*watchHistory) = (*watchHistory)->next;
    return to_return;
}

/*-----Suggested movie to Insert next----------*/
void InserAsNext(struct suggested_movie **currFront, struct movie *Movie)
{
    struct suggested_movie *to_add = (struct suggested_movie *)malloc(sizeof(struct suggested_movie));
    if (!to_add)
    {
        printf("Malloc faild \n");
        return;
    }

    to_add->info.mid = Movie->info.mid;
    to_add->info.year = Movie->info.year;
    to_add->next = NULL;
    to_add->prev = NULL;

    if (*currFront != NULL) // if its not the first node of the list
    {
        (*currFront)->next = to_add;
    }
    else
    {
        *currFront = to_add;
    }

    return;
}
/*------Advance Next Connection---------------*/
void AdvanceNext(struct suggested_movie **currFront)
{
    if ((*currFront)->next != NULL) // if it is the first insertion has no next node
    {
        (*currFront)->next->prev = *currFront;
        (*currFront) = (*currFront)->next;
    }
}
/*-----Suggested Prev movie Insertion----------*/
void InserAsPrev(struct suggested_movie **currBack, struct movie *Movie)
{
    struct suggested_movie *to_add = (struct suggested_movie *)malloc(sizeof(struct suggested_movie));
    if (!to_add)
    {
        printf("Malloc faild \n");
        return;
    }

    to_add->info.mid = Movie->info.mid;
    to_add->info.year = Movie->info.year;
    to_add->next = NULL;
    to_add->prev = NULL;

    if (*currBack != NULL) // if it is not the last node of the list
    {
        (*currBack)->prev = to_add;
    }
    else
    {
        *currBack = to_add;
    }
    return;
}

/*-----Advance Prev Connection----------------*/
void AdvancePrev(struct suggested_movie **currBack)
{
    if ((*currBack)->prev != NULL) // if it is the first insertion has no next node
    {
        (*currBack)->prev->next = *currBack;
        (*currBack) = (*currBack)->prev;
    }
}

/*------Last link ---------------------------*/
void LinkIfNecessary(struct suggested_movie **currFront, struct suggested_movie **currBack)
{
    if ((*currFront) != NULL)
    {
        if ((*currBack) != NULL)
        {
            (*currFront)->next = (*currBack);
            (*currBack)->prev = (*currFront);
        }
        else // if there is no Tail (only one Movie was added)
        {
            (*currBack) = (*currFront);
        }
    }
}

// Creating pointer that points to a node with a copy of movieToCopy's data
struct movie *copy_moive(struct movie *movieToCopy)
{
    struct movie *movieCopy = (struct movie *)malloc(sizeof(struct movie));
    if (!movieCopy)
    {
        printf("Malloc Failed \n");
        return NULL;
    }

    movieCopy->info.mid = movieToCopy->info.mid;
    movieCopy->info.year = movieToCopy->info.year;
    movieCopy->next = NULL;
    return movieCopy;
}

/*Searching for user with the giver uid, returns a pointer to that user or the address
of the Sentinel node if the user is not found*/
struct user *Searching_user(int uid)
{
    struct user *Uaccess = Users;
    while (Uaccess != &sen && Uaccess->uid != uid)
    {
        Uaccess = Uaccess->next;
    }
    return Uaccess;
}

/*---Finding the node with year greater or equal the given year*/
struct movie *FilteredYear(struct movie *access, unsigned year)
{
    while (access != NULL && access->info.year < year)
    {
        access = access->next;
    }
    return access;
}
/*---Creating new node with a copy of the info,from the node we want to add to and connecting it with the filtered list*/
void setInfo(struct suggested_movie **filtered, struct movie **access, struct suggested_movie **accessFiltered)
{
    struct suggested_movie *getInfo = (struct suggested_movie *)malloc(sizeof(struct suggested_movie));
    if (!getInfo)
    {
        printf("Malloc failed\n");
        return;
    }
    getInfo->next = NULL;
    getInfo->prev = NULL;
    getInfo->info.mid = (*access)->info.mid;
    getInfo->info.year = (*access)->info.year;
    if (*filtered == NULL)
    {
        *filtered = getInfo;
    }
    else
    {
        (*accessFiltered)->next = getInfo;
        (*accessFiltered)->next->prev = *accessFiltered;
    }
    *accessFiltered = getInfo;
    free(access);
}

/*-------------------Printing "<uid_1>,<uid_2>...,<uid_n>)"-------------------*/
void print_uid()
{
    struct user *access = Users;
    while (access != NULL && access->uid != -1)
    {
        printf("<%d>,", access->uid);
        access = access->next;
    }
}

/*----------------Printing Mid for movies"<mid_1>,<mid_2>...,<mid_n>)"--------*/
void print_mid(struct movie *access)
{
    while (access != NULL)
    {
        printf("<%u>,", access->info.mid);
        access = access->next;
    }
}

/*------------------------------EVENTS-------------------------------------*/

int register_user(int uid)
{
    // check for unsigned
    struct user *access = Users;
    struct user *to_regi = (struct user *)malloc(sizeof(struct user));
    if (!to_regi) // if the malloc failed
    {
        printf("Malloc failed\n");
        return -1;
    }
    if (uid == -1) // if uid is the same with sentinels
    {
        printf("Users cannot have -1 uid \n\n");
        return -1;
    }

    to_regi->uid = uid;
    // initialize pointers
    to_regi->suggestedHead = NULL;
    to_regi->suggestedTail = NULL;
    to_regi->watchHistory = NULL;

    struct user *last = access;
    if (Users == &sen) /*first node of the list*/
    {
        to_regi->next = Users;
        Users = to_regi;
    }

    while (access != &sen) // finding the sentinel and checking if uid already exists
    {

        if (access->uid == uid) // if uid already exists
        {
            printf("User with id = %u already exists\n \n", uid);
            return -1;
        }
        last = access;
        access = access->next;
    }

    last->next = to_regi;
    to_regi->next = &sen; // List is not shorted, each registration made at the end of the list

    printf("R <uid>\n  Users =");
    print_uid();
    printf("\n DONE\n");

    return 0;
}

void unregister_user(int uid)
{
    struct user *access = Users;
    struct user *prev = access; // to save previous user node

    while (access != &sen && access->uid != uid) // finding the sentinel or the user with the given uid
    {
        prev = access;
        access = access->next;
    }
    if (access == &sen)
    {
        printf("User with id = %u does not exist \n", uid);
        return;
    }
    if (access == Users)
    {
        Users = access->next;
    }
    else
    {
        prev->next = access->next;
    }

    access->suggestedHead = NULL;
    access->suggestedTail = NULL;
    access->watchHistory = NULL;
    access->next = NULL;
    free(access);
    printf("U <uid>\n  Users =");
    print_uid();
    printf("\n DONE\n");

    return;
}

int add_new_movie(unsigned mid, movieCategory_t category, unsigned year)
{
    struct new_movie *to_add = (struct new_movie *)malloc(sizeof(struct new_movie));
    if (!to_add) // if the malloc failed
    {
        printf("Malloc failed\n");
        return -1;
    }
    if (category > 5 || category < 0)
    {
        printf("Movie category does not exist \n");
        return -1;
    }
    if (mid < 0) // optional
    {
        printf("Movie mid cannot be negative \n");
        return -1;
    }
    to_add->category = category;
    to_add->info.mid = mid;
    to_add->info.year = year;
    to_add->next = NULL;
    // adding the movie to the list
    struct new_movie *access = NMovies;
    if (NMovies == NULL) // new_movie list is empty
    {
        NMovies = to_add;
    }
    else
    {
        struct new_movie *last = access;
        while (access != NULL && access->info.mid <= mid) // access->info.mid <= mid, using less or equal
        // if they are equal meaning the given mid already exists and the if check becoming true
        {
            if (access->info.mid == mid) // if there is a movie with the same mid
            {
                printf("Movie with mid = %u already exists \n\n", mid);
                return -1;
            }
            last = access;
            access = access->next;
        }
        if (access == NMovies)
        {
            to_add->next = NMovies;
            NMovies = to_add;
        }
        else
        {
            to_add->next = last->next;
            last->next = to_add;
        }
    }
    access = NMovies;
    printf("A<mid><category><year>\n  New movies =");
    while (access != NULL)
    {
        printf("<%u,", access->info.mid);
        print_enum((int)access->category);
        printf(",%u>,", access->info.year);
        access = access->next;
    }
    printf("\nDONE \n\n");
    return 0;
}

void distribute_new_movies(void)
{
    int cat, Mid;

    struct movie *access;
    struct movie *prev[6] = {NULL};

    while (NMovies != NULL)
    {
        struct movie *to_dist = (struct movie *)malloc(sizeof(struct movie));
        if (!to_dist) // if the malloc failed
        {
            printf("Malloc failed\n");
            return;
        }
        cat = (int)NMovies->category;
        Mid = NMovies->info.mid;
        to_dist->info.mid = Mid;
        to_dist->info.year = NMovies->info.year;
        to_dist->next = NULL;

        if (array[cat] == NULL)
        {
            prev[cat] = to_dist;
            array[cat] = to_dist;
        }
        else
        {
            prev[cat]->next = to_dist;
            prev[cat] = to_dist;
        }
        struct new_movie *next = NMovies->next;
        free(NMovies);
        NMovies = next;
    }

    printf("D\n Categorized Movies:");
    for (int i = 0; i < 6; i++)
    {
        printf("\n  ");
        print_enum(i);
        printf(": ");
        access = array[i];
        print_mid(access);
    }
    printf("\nDONE\n\n");
}

int watch_movie(int uid, unsigned mid)
{
    /*----------Searching the Movie with mid--------------------*/
    struct movie *access;
    int i;
    for (i = 0; i < 6; i++) // checking for all categories
    {
        access = array[i];
        while (access != NULL && access->info.mid != mid)
        {
            access = access->next;
        }
        if (access != NULL && access->info.mid == mid)
        {
            break;
        }
    }
    /*if the movie with the given mid is not found*/
    if (access == NULL || access->info.mid != mid)
    {
        printf("Movie with mid = %u did not found \n", mid);
        return -1;
    }
    /*----------Searching the User with the given uid--------------------*/
    struct user *Uaccess = Searching_user(uid);
    /*---------if the User is not found------------------*/
    if (Uaccess == &sen)
    {
        printf("User with uid = %u did not found \n", uid);
        return -1;
    }

    /*------------Movie and user exist ------------------- */

    struct movie *WatchedMovie = copy_moive(access); /*Copying the data from the movie to a new node*/
    if (Uaccess->watchHistory == NULL)               /*If this is the first movie of the user*/
    {
        Uaccess->watchHistory = WatchedMovie;
    }
    else
    {
        WatchedMovie->next = Uaccess->watchHistory;
        Uaccess->watchHistory = WatchedMovie;
    }
    /*---Print the W event---------------------*/
    printf("W <%u><%u>\n  User <%u> Watch History =", uid, mid, uid);
    access = Uaccess->watchHistory;
    print_mid(access);
    printf("\nDONE\n\n");

    return 0;
}

int suggest_movies(int uid)
{
    /*----Searching the user with uid--------------*/
    struct user *Uaccess = Searching_user(uid);
    /*---------if the User is not found------------------*/
    if (Uaccess == &sen)
    {
        printf("User with uid = %u is not found \n", uid);
        return -1;
    }
    struct suggested_movie *oldHead = NULL;
    struct suggested_movie *oldTail = NULL;
    // checking for an old suggest list
    if (Uaccess->suggestedTail != NULL)
    {
        oldHead = Uaccess->suggestedHead;
        oldTail = Uaccess->suggestedTail;
        Uaccess->suggestedTail = NULL;
        Uaccess->suggestedHead = NULL;
    }

    struct suggested_movie *currFront = Uaccess->suggestedHead;
    struct suggested_movie *currBack = Uaccess->suggestedTail;
    struct user *Found = Uaccess;
    int counter = 1;
    Uaccess = Users;
    struct movie *Movie;

    while (Uaccess != &sen)
    {

        if (Uaccess->uid != uid)
        {

            Movie = pop(&Uaccess->watchHistory);
            if (Movie != NULL) // if the watched history of the users is not empty
            {
                if (counter % 2 != 0)
                {

                    InserAsNext(&currFront, Movie);
                    if (Found->suggestedHead == NULL)
                    {
                        Found->suggestedHead = currFront;
                    }

                    AdvanceNext(&currFront);
                }
                else
                {
                    InserAsPrev(&currBack, Movie);
                    if (Found->suggestedTail == NULL)
                    {
                        Found->suggestedTail = currBack;
                    }
                    AdvancePrev(&currBack);
                }
                counter++;
                free(Movie);
            }
        }
        Uaccess = Uaccess->next;
    }
    LinkIfNecessary(&currFront, &currBack);
    if (oldHead != NULL)
    {
        LinkIfNecessary(&oldTail, &Found->suggestedHead);
        Found->suggestedHead = oldHead;
    }

    printf("S<%d>\n   User<%d> Suggested Movies =", uid, uid);

    if (Found->suggestedHead != NULL && Found->suggestedTail == NULL)
    {
        Found->suggestedTail = Found->suggestedHead; // If there is only one Suggested Movie
    }
    else if (Found->suggestedHead == NULL)
    {
        printf("<>\n"); // if there is no suggested movie
    }
    if (Found->suggestedTail != NULL)
    {
        currFront = Found->suggestedHead;

        while (currFront != Found->suggestedTail)
        {
            printf("<%d>,", currFront->info.mid);
            currFront = currFront->next;
        }
        printf("<%d>\n", currFront->info.mid); // for the last movie(suggestedTail)
    }
    printf("\nDONE\n\n");

    return 0;
}

int filtered_movie_search(int uid, movieCategory_t category1, movieCategory_t category2, unsigned year)
{

    /*Using struct suggested_movie as double linked list for the filtered Movie Search */
    struct suggested_movie *filtered = (struct suggested_movie *)malloc(sizeof(struct suggested_movie));
    if (!filtered)
    {
        printf("Malloc failed\n");
        return -1;
    }
    filtered = NULL;
    struct suggested_movie *accessFiltered = filtered;
    /*----Searching the user with uid--------------*/
    struct user *Uaccess = Searching_user(uid);
    /*---------if the User is not found------------------*/
    if (Uaccess == &sen)
    {
        printf("User with uid = %u is not found \n", uid);
        return -1;
    }

    struct movie *access1 = array[(int)category1];
    struct movie *access2 = array[(int)category2];
    if ((int)category1 == (int)category2)
    {
        access2 = NULL;
    }
    while (access1 != NULL || access2 != NULL)
    {
        access1 = FilteredYear(access1, year);
        access2 = FilteredYear(access2, year);
        if (access1 == NULL && access2 != NULL)
        {
            setInfo(&filtered, &access1, &accessFiltered);
            access2 = access2->next;
        }
        else if (access2 == NULL && access1 != NULL)
        {
            setInfo(&filtered, &access1, &accessFiltered);
            access1 = access1->next;
        }
        else
        {
            if (access1->info.mid < access2->info.mid)
            {

                setInfo(&filtered, &access1, &accessFiltered);
                access1 = access1->next;
            }
            else
            {

                setInfo(&filtered, &access2, &accessFiltered);
                access2 = access2->next;
            }
        }
    }
    if (Uaccess->suggestedHead == NULL)
    {
        Uaccess->suggestedHead = filtered;
        Uaccess->suggestedTail = filtered;
    }
    else
    {
        Uaccess->suggestedTail->next = filtered;
        Uaccess->suggestedTail->next->prev = Uaccess->suggestedTail;
    }
    printf("F<%d><", uid);
    print_enum((int)category1);
    printf("><");
    print_enum((int)category2);
    printf("><%u>\n  User<%d> Suggested Movies =", year, uid);
    accessFiltered = Uaccess->suggestedHead;
    while (accessFiltered != NULL)
    {
        printf("<%u>,", accessFiltered->info.mid);
        accessFiltered = accessFiltered->next;
    }
    printf("\n DONE\n\n");

    return 0;
}

void take_off_movie(unsigned mid)
{
    /*----------Searching the Movie with mid--------------------*/
    struct movie *access, *prev;
    int i;
    for (i = 0; i < 6; i++) // checking for all categories
    {
        access = array[i];
        prev = access;
        while (access != NULL && access->info.mid != mid)
        {
            prev = access;
            access = access->next;
        }
        if (access != NULL && access->info.mid == mid)
        {
            break;
        }
    }
    /*if the movie with the given mid is not found*/
    if (access == NULL || access->info.mid != mid)
    {
        printf("Movie with mid = %u is not found \n", mid);
        return;
    }

    /*If the movie was found, i = category*/
    printf("T<%u>\n ", mid);
    struct user *Uaccess = Users;

    while (Uaccess != &sen)
    {

        struct suggested_movie *suggest = Uaccess->suggestedHead;
        struct suggested_movie *last = suggest;

        while (suggest != NULL)
        {
            if (suggest->info.mid == mid)
            {

                if (suggest == Uaccess->suggestedHead)
                {
                    Uaccess->suggestedHead = suggest->next;
                }
                else if (suggest == Uaccess->suggestedTail)
                {
                    Uaccess->suggestedTail = last;
                }
                else
                {
                    last->next = suggest->next;
                    suggest->next->prev = last;
                }

                suggest->next = NULL;
                suggest->prev = NULL;
                free(suggest);
                printf("<%u> removed from <%d> suggested list.\n ", mid, Uaccess->uid);
                break;
            }
            last = suggest;
            //--
            if (last == Uaccess->suggestedTail)
            {
                break;
            }

            suggest = suggest->next;
        }

        Uaccess = Uaccess->next;
    }
    if (array[i] == access)
    {
        array[i] = access->next;
    }
    else
    {
        prev->next = access->next;
    }
    access->next = NULL;
    free(access);
    printf(" ");
    print_enum(i);
    printf("=");
    if (array[i] != NULL)
    {
        access = array[i];
        print_mid(access);
    }
    else
    {
        printf("<>\n"); // is empty
    }

    printf("\nDONE\n");
}

void print_movies(void)
{
    printf("M\n Categorized Movies:\n  ");
    for (int i = 0; i < 6; i++)
    {
        print_enum(i);
        printf(":");
        struct movie *access = array[i];
        print_mid(access);
        printf("\n  ");
    }
    printf("\nDONE\n");
}

void print_users(void)
{
    struct user *Uaccess = Users;
    printf("P \nUsers:\n");
    while (Uaccess != &sen)
    {
        printf(" <%d>:\n   Suggested:", Uaccess->uid);
        struct suggested_movie *suggest = Uaccess->suggestedHead;
        while (suggest != NULL)
        {
            printf("<%d>,", suggest->info.mid);
            suggest = suggest->next;
        }
        printf("\n   Watch History:");
        struct movie *Haccess = Uaccess->watchHistory;
        print_mid(Haccess);
        printf("\n");
        Uaccess = Uaccess->next;
    }
    printf("\nDONE\n");
}
