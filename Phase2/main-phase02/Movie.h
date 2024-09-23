/*****************************************************
 * @file   Movie.h                                    *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Header File for Data Structures (CS240b)   *
 * Project: Winter 2023						         *
 *****************************************************/
#ifndef MOVIE_FILE
#define MOVIE_FILE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

/*You can use this enum or you can choose to represent each category as an Integer. Its the same*/
typedef enum
{
	HORROR,
	SCIFI,
	DRAMA,
	ROMANCE,
	DOCUMENTARY,
	COMEDY
} Category_t;

/**
 * Structure defining a node of a new_realeases binary tree (dendro newvn kykloforiwn)
 */
typedef struct new_movie
{
	int category;		  /* The category of the movie*/
	int movieID;		  /* The movie identifier*/
	int year;			  /* The year movie released*/
	int watchedCounter;	  /* How many users rate the movie*/
	int sumScore;		  /* The sum of the ratings of the movie*/
	struct new_movie *lc; /* Pointer to the node's left child*/
	struct new_movie *rc; /* Pointer to the node's right child*/
} new_movie_t;

/**
 *Structure defining a node of movie binary tree (dendro tainiwn kathgorias)
 */
typedef struct movie
{
	int movieID;		/* The movie identifier*/
	int year;			/* The year movie released*/
	int watchedCounter; /* How many users rate the movie*/
	int sumScore;		/* The sum of the ratings of the movie*/
	struct movie *lc;	/* Pointer to the node's left child*/
	struct movie *rc;	/* Pointer to the node's right child*/
} movie_t;

/**
 * Structure defining movie_category
 */

typedef struct movie_category
{
	movie_t *movie;
	movie_t *sentinel;
} movieCategory_t;

/**
 * Structure defining a node of user_movie for history doubly linked binary
 * tree (dentro istorikou)
 */
typedef struct user_movie
{
	int movieID;			   /* The movie identifier*/
	int category;			   /* The category of the movie*/
	int score;				   /* The score of the movie*/
	struct user_movie *parent; /* Pointer to the node's parent*/
	struct user_movie *lc;	   /* Pointer to the node's left child*/
	struct user_movie *rc;	   /* Pointer to the node's right child*/
} userMovie_t;

/**
 * Structure defining a node of users' hashtable (pinakas katakermatismou
 * xrhstwn)
 */
typedef struct user
{
	int userID;			  /* The user's identifier*/
	userMovie_t *history; /* A doubly linked binary tree with the movies watched by the user*/
	struct user *next;	  /* Pointer to the next node of the chain*/
} user_t;

extern movieCategory_t *categoryArray[6]; /* The categories array (pinakas kathgoriwn)*/
extern int NumOfCategor;				  /* The number of categories*/
extern new_movie_t *new_releases;		  /* New releases simply-linked binary tree*/
extern user_t **user_hashtable_p;		  /* The users hashtable. This is an array of chains (pinakas katakermatismoy xrhstwn)*/
extern int hashtable_size;				  /* The size of the users hashtable, parsed from the command line (>0)*/
extern int max_users;					  /* The maximum number of registrations (users)*/
extern int max_id;						  /* The maximum account ID */
extern int primes_g[170];
// Global int variables  for the hush function
extern int p;
extern int a;
extern int b;

/**
 * @brief Creates a new user.
 * Creates a new user with userID as its identification.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

int register_user(int userID);

/**
 * @brief Deletes a user.
 * Deletes a user with userID from the system, along with users' history tree.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

int unregister_user(int userID);

/**
 * @brief Add new movie to new release binary tree.
 * Create a node movie and insert it in 'new release' binary tree.
 *
 * @param movieID The new movie identifier
 * @param category The category of the movie
 * @param year The year movie released
 *
 * @return 1 on success
 *         0 on failure
 */

int add_new_movie(int movieID, int category, int year);

/**
 * @brief Distribute the movies from new release binary tree to the array of categories.
 *
 * @return 1 on success
 *         0 on failure
 */

int distribute_movies(void);

/**
 * @brief User rates the movie with identification movieID with score
 *
 * @param userID The identifier of the user
 * @param category The Category of the movie
 * @param movieID The identifier of the movie
 * @param score The score that user rates the movie with id movieID
 *
 * @return 1 on success
 *         0 on failure
 */

int watch_movie(int userID, int category, int movieID, int score);

/**
 * @brief Identify the best rating score movie and cluster all the movies of a category.
 *
 * @param userID The identifier of the user
 * @param score The minimum score of a movie
 *
 * @return 1 on success
 *         0 on failure
 */

int filter_movies(int userID, int score);

/**
 * @brief Find the median score that user rates movies.
 *
 * @param userID The identifier of the user
 *
 * @return 1 on success
 *         0 on failure
 */

int user_stats(int userID);

/**
 * @brief Search for a movie with identification movieID in a specific category.
 *
 * @param movieID The identifier of the movie
 * @param category The category of the movie
 *
 * @return 1 on success
 *         0 on failure
 */

int search_movie(int movieID, int category);

/**
 * @brief Prints the movies in movies categories array.
 * @return 1 on success
 *         0 on failure
 */

int print_movies(void);

/**
 * @brief Prints the users hashtable.
 * @return 1 on success
 *         0 on failure
 */

int print_users(void);

// initializes global variables
void initGlobal();

// Hash function
int hashFunction(int k);

// Searches for the user with the given id
user_t *findUser(int userID);

// Prints all users' id, of the given chain
void printUsersChain(int j);

void printCategory(int num);

// Inorder Traversal for New Releases Tree
void InOrderPrint(new_movie_t *p);

// counts how many movies of the given category there are
int movieCategorCounter(int categor, new_movie_t *p);

// converts a binary tree to a sorted array
void TreeToArray(int cat, int length, int *index, movie_t **array, new_movie_t *p);

// Inorder Traversal for each category Tree
void InOrderPrintCat(int i, movie_t *p);

// convers a sorted array to a binary search tree
movie_t *arrayToTree(movie_t **array, int n);

// Inserts nodes into the binary search tree
movie_t *BinInsert(movie_t **root, movie_t *node);

void arrayBTS(int start, int end, movie_t **root, movie_t **array);

// Inserts nodes into History Tree of the given user
userMovie_t *HistoryInsert(user_t *UsersAccess, userMovie_t *to_add);

// Inorder Traversal for the given History Tree
void printUserMovies(userMovie_t *root);

// counts the number of the movies that have score greater or equal to the given one
void movieCount(int score, movie_t *root, int *sum);

// Stores the pointer of the movies with average score greater or equal to the given one
void findMovies(movie_t **array, int *index, int score, movie_t *root);

void InitialeHeap(movie_t **array, int n);
void swap(movie_t **a, movie_t **b);
void HeapSort(movie_t **array, int n);
void Heapify(movie_t **array, int i, int n);
int AverageScore(movie_t *node);

userMovie_t *FindNextLeaf(userMovie_t *node);

#endif
