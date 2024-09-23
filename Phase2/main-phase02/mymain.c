/*****************************************************
 * @file   main.c                                    *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Main Function for Data Structures (CS240b) *
 * Project: Winter 2023						         *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Movie.h"

#define BUFFER_SIZE 1024 /**< Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */

int hashtable_size; /** The size of the users hashtable (>0) */
int max_users;      /** The maximum number of registrations (users) */
int max_id;         /** The maximum user ID */

// This is a very conservative progress on the hashtable. Our purpose
// is to force many rehashes to check the stability of the code.
int primes_g[170] = {5, 7, 11, 13, 17, 19, 23, 29, 31, 37,
                     41, 43, 47, 53, 59, 61, 67, 71, 73, 79,
                     83, 89, 97, 101, 103, 107, 109, 113, 127, 131,
                     137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
                     191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
                     241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
                     307, 311, 313, 317, 331, 337, 347, 349, 353, 359,
                     367, 373, 379, 383, 389, 397, 401, 409, 419, 421,
                     431, 433, 439, 443, 449, 457, 461, 463, 467, 479,
                     487, 491, 499, 503, 509, 521, 523, 541, 547, 557,
                     563, 569, 571, 577, 587, 593, 599, 601, 607, 613,
                     617, 619, 631, 641, 643, 647, 653, 659, 661, 673,
                     677, 683, 691, 701, 709, 719, 727, 733, 739, 743,
                     751, 757, 761, 769, 773, 787, 797, 809, 811, 821,
                     823, 827, 829, 839, 853, 857, 859, 863, 877, 881,
                     883, 887, 907, 911, 919, 929, 937, 941, 947, 953,
                     967, 971, 977, 983, 991, 997, 1009, 1013, 1019, 1021};

user_t **user_hashtable_p;         /* The users hashtable. This is an array of chains (pinakas katakermatismoy xrhstwn)*/
new_movie_t *new_releases;         /* New releases simply-linked binary tree*/
movieCategory_t *categoryArray[6]; /* The categories array (pinakas kathgoriwn)*/
int NumOfCategor = 6;

int main()
{
    max_id = 784;
    max_users = 5;
    initGlobal(); // reminder na alla3w name
    register_user(784);
    register_user(171);
    register_user(667);
    register_user(223);
    register_user(396);
    register_user(196);

    print_users();

    unregister_user(784);
    unregister_user(171);
    unregister_user(171);
    unregister_user(667);
    unregister_user(223);
    unregister_user(396);
    unregister_user(223);
    print_users();

    exit(1);

    add_new_movie(1387, 4, 2011);
    add_new_movie(908, 1, 2007);
    add_new_movie(2129, 4, 2004);
    add_new_movie(3229, 2, 2011);
    add_new_movie(3524, 3, 2004);
    add_new_movie(1415, 3, 2008);
    add_new_movie(2503, 0, 2002);
    add_new_movie(1656, 2, 2014);
    add_new_movie(323, 3, 2002);
    add_new_movie(2883, 5, 2000);
    distribute_movies();

    // watch_movie(171, 3, 1415, 1);
    watch_movie(171, 3, 3524, 7);
    watch_movie(784, 1, 908, 5);
    watch_movie(171, 3, 323, 6);

    watch_movie(171, 4, 2129, 9);
    watch_movie(223, 4, 2129, 0);

    filter_movies(171, 5);
    user_stats(223);
    print_movies();
    print_users();
    printf("\n\t\t\t\t End Main \n");
    return 0;
}
// int main(int argc, char **argv)
// {
//     max_users = -1;
//     max_id = -1;
//     int flag = 0;
//     FILE *fin = NULL;
//     char buff[BUFFER_SIZE], event;

//     /* Check command buff arguments */
//     if (argc != 2)
//     {
//         fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
//         return EXIT_FAILURE;
//     }

//     /* Open input file */
//     if ((fin = fopen(argv[1], "r")) == NULL)
//     {
//         fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
//         perror("Opening test file\n");
//         return EXIT_FAILURE;
//     }

//     /* Read input file buff-by-buff and handle the events */
//     while (fgets(buff, BUFFER_SIZE, fin))
//     {
//         if (flag == 0 && max_id != -1 && max_users != -1)
//         {
//             initGlobal();
//             flag = 1;
//         }

//         DPRINT("Event: %s \n", buff);

//         switch (buff[0])
//         {

//         /* Comment */
//         case '#':
//             break;
//         /* max_users */
//         case '0':
//         {
//             sscanf(buff, "%c %u", &event, &max_users);
//             DPRINT("max users: %u\n", max_users);
//             break;
//         }
//         /* max_id */
//         case '1':
//         {
//             sscanf(buff, "%c %u", &event, &max_id);
//             DPRINT("max id: %u\n", max_id);
//             break;
//         }
//         /* Event R : R <userID> - Register user. */
//         case 'R':
//         {
//             int userID;
//             sscanf(buff, "%c %d", &event, &userID);
//             DPRINT("%c %d\n", event, userID);
//             if (register_user(userID))
//             {
//                 DPRINT("%c succeeded\n", event);
//             }
//             else
//             {
//                 fprintf(stderr, "%c failed\n", event);
//             }

//             break;
//         }
//         /* Event U : U <userID> - Unregister user. */
//         case 'U':
//         {
//             int userID;
//             sscanf(buff, "%c %d", &event, &userID);
//             DPRINT("%c %d\n", event, userID);

//             if (unregister_user(userID))
//             {
//                 DPRINT("%c %d succeeded\n", event, userID);
//             }
//             else
//             {
//                 fprintf(stderr, "%c %d failed\n", event, userID);
//             }

//             break;
//         }
//         /* Event A : A <movieID> <category> <year> - Add new movie. */
//         case 'A':
//         {
//             int movieID, category, year;
//             sscanf(buff, "%c %d %d %d", &event, &movieID, &category, &year);
//             DPRINT("%c %d %d %d\n", event, movieID, category, year);

//             if (add_new_movie(movieID, category, year))
//             {
//                 DPRINT("%c %d %d %d succeeded\n", event, movieID, category, year);
//             }
//             else
//             {
//                 fprintf(stderr, "%c %d %d %d failed\n", event, movieID, category, year);
//             }

//             break;
//         }
//         /* Event D : D  - Distribute movies. */
//         case 'D':
//         {
//             sscanf(buff, "%c", &event);
//             DPRINT("%c\n", event);

//             if (distribute_movies())
//             {
//                 DPRINT("%c succeeded\n", event);
//             }
//             else
//             {
//                 fprintf(stderr, "%c failed\n", event);
//             }

//             break;
//         }
//         /* Event W : W <userID ><category><movieID><score> - Watch movie */
//         case 'W':
//         {
//             int userID, movieID, category, score;

//             sscanf(buff, "%c %d %d %d %d", &event, &userID, &category, &movieID, &score);
//             DPRINT("%c %d %d %d %d\n", event, userID, category, movieID, score);

//             if (watch_movie(userID, category, movieID, score))
//             {
//                 DPRINT("%c %d %d %d %d succeeded\n", event, userID, category, movieID, score);
//             }
//             else
//             {
//                 fprintf(stderr, "%c %d %d %d %d failed\n", event, userID, category, movieID, score);
//             }

//             break;
//         }
//         /* Event –	F  <userID ><category1><category2><score> Filter movies */
//         case 'F':
//         {
//             int userID, score;
//             sscanf(buff, "%c %d %d\n", &event, &userID, &score);
//             DPRINT("%c %d %d\n", event, userID, score);

//             if (filter_movies(userID, score))
//             {
//                 DPRINT("%c %d %d succeeded\n", event, userID, score);
//             }
//             else
//             {
//                 fprintf(stderr, "%c %d %d failed\n", event, userID, score);
//             }

//             break;
//         }
//         /* Event Q : Q <userID> - User statistics */
//         case 'Q':
//         {
//             int userID;
//             sscanf(buff, "%c %d\n", &event, &userID);
//             DPRINT("%c %d\n", event, userID);

//             if (user_stats(userID))
//             {
//                 DPRINT("%c %d succeeded\n", event, userID);
//             }
//             else
//             {
//                 fprintf(stderr, "%c %d failed\n", event, userID);
//             }

//             break;
//         }
//         /* Event I : I <movieID> <category> - Search movie */
//         case 'I':
//         {
//             int movieID, category;
//             sscanf(buff, "%c %d %d\n", &event, &movieID, &category);
//             DPRINT("%c %d %d\n", event, movieID, category);

//             if (search_movie(movieID, category))
//             {
//                 DPRINT("%c %d %d succeeded\n", event, movieID, category);
//             }
//             else
//             {
//                 fprintf(stderr, "%c %d %d failed\n", event, movieID, category);
//             }

//             break;
//         }
//         /* Event M : M  - Print movies */
//         case 'M':
//         {
//             sscanf(buff, "%c", &event);
//             DPRINT("%c\n", event);

//             if (print_movies())
//             {
//                 DPRINT("%c succeeded\n", event);
//             }
//             else
//             {
//                 fprintf(stderr, "%c failed\n", event);
//             }

//             break;
//         }
//         /* Event P : P  - Print users */
//         case 'P':
//         {
//             sscanf(buff, "%c", &event);
//             DPRINT("%c\n", event);

//             if (print_users())
//             {
//                 DPRINT("%c succeeded\n", event);
//             }
//             else
//             {
//                 fprintf(stderr, "%c failed\n", event);
//             }

//             break;
//         }
//         /* Empty line */
//         case '\n':
//             break;

//         /* Ignore everything else */
//         default:
//             DPRINT("Ignoring buff: %s \n", buff);
//             break;
//         }
//     }

//     return (EXIT_SUCCESS);
// }