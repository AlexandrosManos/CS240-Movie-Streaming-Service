/*****************************************************
 * @file   Movie.c                                   *
 * @author Paterakis Giorgos <geopat@csd.uoc.gr>     *
 *                                                   *
 * @brief Implementation for Movie.h 				 *
 * Project: Winter 2023						         *
 *****************************************************/
#include "Movie.h"
#include <time.h>

// Global int variables  for the hush function
int a;
int b;
int p;

/**
 * @brief Creates a new user.
 * Creates a new user with userID as its identification.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

int register_user(int userID)
{
	int index = hashFunction(userID);
	user_t *NewUser = (user_t *)malloc(sizeof(user_t));
	if (!NewUser)
	{
		printf("Malloc failed \n");
		return 0;
	}
	NewUser->history = NULL;
	NewUser->next = NULL;
	NewUser->userID = userID;

	if (user_hashtable_p[index] == NULL) // first node of the chain
	{
		user_hashtable_p[index] = NewUser;
	}
	else
	{
		user_t *access = user_hashtable_p[index];
		while (access->next != NULL && access->userID != userID)
		{
			access = access->next;
		}
		if (access->userID == userID)
		{
			printf("User with ID <%d> already exists \n", userID);
			return 0;
		}

		access->next = NewUser;
	}
	printf("R <%d>\n", userID);
	printUsersChain(index);
	printf("Done\n\n");

	return 1;
}

/**
 * @brief Deletes a user.
 * Deletes a user with userID from the system, along with users' history tree.
 *
 * @param userID The new user's identification
 *
 * @return 1 on success
 *         0 on failure
 */

int unregister_user(int userID)
{
	int index = hashFunction(userID);
	user_t *prev = user_hashtable_p[index];		 // the previous node of the given user
	user_t *to_Delete = user_hashtable_p[index]; // the user we want to delete
	while (to_Delete != NULL && to_Delete->userID != userID)
	{
		prev = to_Delete;
		to_Delete = to_Delete->next;
	}
	if (to_Delete == NULL)
	{
		printf("User with ID = <%d> not found\n", userID);
		return 0;
	}
	if (to_Delete == user_hashtable_p[index]) // first node of the chain
	{
		user_hashtable_p[index] = to_Delete->next;
	}
	else
	{
		prev->next = to_Delete->next;
	}
	free(to_Delete);
	printf("U <%d> \n", userID);
	printUsersChain(index);
	printf("Done\n\n");
	return 1;
}

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

int add_new_movie(int movieID, int category, int year)
{
	if (category < 0 || category >= NumOfCategor)
	{
		printf("Unknown category\n");
		return 0;
	}
	new_movie_t *access = new_releases;
	new_movie_t *prev = NULL;
	while (access)
	{
		if (access->movieID == movieID)
		{
			printf("Movie with movieID <%d> already existis \n", movieID);
			return 0;
		}
		prev = access;
		if (access->movieID > movieID)
		{
			access = access->lc;
		}
		else
		{
			access = access->rc;
		}
	}
	new_movie_t *to_add = (new_movie_t *)malloc(sizeof(new_movie_t));
	if (!to_add)
	{
		printf("Malloc failed\n");
		return 0;
	}
	to_add->category = category;
	to_add->movieID = movieID;
	to_add->year = year;
	to_add->watchedCounter = 0;
	to_add->sumScore = 0;
	to_add->lc = NULL;
	to_add->rc = NULL;
	if (prev == NULL) // the root of the binary tree
	{
		new_releases = to_add;
	}
	else if (prev->movieID > movieID)
	{
		prev->lc = to_add;
	}
	else
	{
		prev->rc = to_add;
	}
	printf("A <%d> <", movieID);
	printCategory(category);
	printf("> <%d>\n New releases Tree:\n", year);
	printf("\t<new_releases>: ");
	InOrderPrint(new_releases);
	printf("\nDone\n");
	return 1;
}

/**
 * @brief Distribute the movies from new release binary tree to the array of categories.
 *
 * @return 0 on success
 *         1 on failure
 */

int distribute_movies(void)
{
	int i;
	for (i = 0; i < NumOfCategor; i++)
	{
		int numOfMovies = movieCategorCounter(i, new_releases);

		if (!numOfMovies) // no movies in this category
		{
			continue;
		}
		movie_t **array = (movie_t **)malloc(numOfMovies * sizeof(movie_t *));
		if (array == NULL)
		{
			printf("Malloc failed\n");
			return 0;
		}
		int index = 0;
		TreeToArray(i, numOfMovies, &index, array, new_releases);
		categoryArray[i]->movie = arrayToTree(array, numOfMovies);
	}
	printf("\n-----------Distribute movies-------------------\n");
	for (i = 0; i < NumOfCategor; i++)
	{
		printf("\nCategory <");
		printCategory(i);
		printf(">: \n");
		InOrderPrintCat(i, categoryArray[i]->movie);
	}
	printf("\nDONE");
	printf("\n--------------------------------------------\n");
	return 1;
}

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

int watch_movie(int userID, int category, int movieID, int score)
{
	//-------Checks--------------------------------------
	if (category < 0 || category > 5)
	{
		printf("Unknown Movie Category\n");
		return 0;
	}
	if (score < 1 || score > 10)
	{
		printf("Invalid score \n");
		return 0;
	}
	user_t *UsersAccess = findUser(userID);
	if (UsersAccess == NULL)
	{
		printf("User not found \n");
		return 0;
	}
	//-------------------------------------------------
	movie_t *access = categoryArray[category]->movie;
	while (access != NULL && access != categoryArray[category]->sentinel)
	{
		if (access->movieID == movieID)
		{
			break;
		}
		else if (access->movieID > movieID)
		{
			access = access->lc;
		}
		else
		{
			access = access->rc;
		}
	}
	if (access == NULL || access->movieID != movieID)
	{
		printf("On Category <");
		printCategory(category);
		printf(">, there is no movie with ID <%d>\n", movieID);
		return 0;
	}
	access->watchedCounter++;
	access->sumScore += score;
	userMovie_t *to_add = (userMovie_t *)malloc(sizeof(userMovie_t));
	if (!to_add)
	{
		printf("Malloc failed\n");
		return 0;
	}
	to_add->category = category;
	to_add->score = score;
	to_add->movieID = movieID;
	to_add->lc = NULL;
	to_add->rc = NULL;
	to_add->parent = NULL;

	UsersAccess->history = HistoryInsert(UsersAccess, to_add);
	printf("W <%d> <", userID);
	printCategory(category);
	printf("><%d><%d>\n", movieID, score);
	printf("History Tree of user <%d>:\n", userID);
	printUserMovies(UsersAccess->history);
	printf("Done\n");

	return 1;
}

/**
 * @brief Identify the best rating score movie and cluster all the movies of a category.
 *
 * @param userID The identifier of the user
 * @param score The minimum score of a movie
 *
 * @return 1 on success
 *         0 on failure
 */

int filter_movies(int userID, int score)
{

	user_t *UsersAccess = findUser(userID);
	if (UsersAccess == NULL)
	{
		printf("User not found \n");
		return 0;
	}
	int i, numMovies = 0;
	movie_t *access;
	for (i = 0; i < NumOfCategor; i++) // for each category
	{
		access = categoryArray[i]->movie;
		if (access == NULL) // no movie in this category
		{
			continue;
		}
		movieCount(score, access, &numMovies);
	}
	printf("\n----------------\n");
	if (numMovies == 0) // no movie found with such high rating
	{
		printf("F <%d><%d>\nNo movie found with such high rating\nDone\n", userID, score);
		return 1; // program did not fail
	}
	movie_t **array = (movie_t **)malloc(numMovies * sizeof(movie_t *));
	if (!array)
	{
		printf("Malloc failed \n");
		return 0;
	}
	int index = 0;
	for (i = 0; i < NumOfCategor; i++)
	{
		access = categoryArray[i]->movie;
		if (access == NULL)
		{
			continue;
		}
		findMovies(array, &index, score, access);
		if (index == numMovies)
		{
			break;
		}
	}
	HeapSort(array, numMovies);
	printf("F<%d><%d>\n\t", userID, score);
	for (i = 0; i < numMovies; i++)
	{
		int average = AverageScore(array[i]); // no check needed
		printf("<%d><%d>, ", array[i]->movieID, average);
	}
	printf("\nDone \n");

	return 1;
}

/**
 * @brief Find movies from categories withn median_score >= score t
 *
 * @param userID The identifier of the user
 * @param category Array with the categories to search.
 * @param score The minimum score the movies we want to have
 *
 * @return 1 on success
 *         0 on failure
 */

int user_stats(int userID)
{
	user_t *UserAccess = findUser(userID);
	if (!UserAccess)
	{
		printf("User Not found\n");
		return 0;
	}
	int counter = 0, ScoreSum = 0;
	userMovie_t *access = UserAccess->history;
	if (access != NULL)
	{
		while (access->lc != NULL)
		{
			access = access->lc;
		}
		while (access->rc != NULL) // if access is not leaf
		{
			while (access->lc != NULL)
			{
				access = access->lc;
			}
			access = access->rc;
		}
		while (access != NULL)
		{
			counter++;
			ScoreSum += access->score;
			access = FindNextLeaf(access);
		}
	}

	printf("Q <%d> ", userID);
	if (counter == 0)
	{
		printf("No reviews from this user \n");
	}
	else
	{
		printf("<%d>\n", ScoreSum / counter);
	}
	printf("Done\n");
	return 1;
}

/**
 * @brief Search for a movie with identification movieID in a specific category.
 *
 * @param movieID The identifier of the movie
 * @param category The category of the movie
 *
 * @return 1 on success
 *         0 on failure
 */

int search_movie(int movieID, int category)
{
	if (category < 0 || category > 5)
	{
		printf("Unknown Movie Category\n");
		return 0;
	}
	movie_t *access = categoryArray[category]->movie;
	while (access != NULL && access != categoryArray[category]->sentinel)
	{
		if (access->movieID == movieID)
		{
			printf("I <");
			printCategory(category);
			printf("> <%d> <%d>\nDONE\n", category, access->year);
			return 1;
		}
		if (access->movieID > movieID)
		{
			access = access->lc;
		}
		else
		{
			access = access->rc;
		}
	}
	printf("On Category <");
	printCategory(category);
	printf(">, there is no movie with ID <%d>\n", movieID);
	return 0;
}

/**
 * @brief Prints the movies in movies categories array.
 * @return 1 on success
 *         0 on failure
 */

int print_movies(void)
{
	printf("M\nMovie Category Array:");
	for (int i = 0; i < NumOfCategor; i++)
	{
		printf("\n\t<%d>: ", i);
		if (categoryArray[i]->movie == NULL)
		{
			continue;
		}
		InOrderPrintCat(i, categoryArray[i]->movie);
	}
	printf("\nDone\n");

	return 1;
}

/**
 * @brief Prints the users hashtable.
 * @return 1 on success
 *         0 on failure
 */

int print_users(void)
{
	printf("P\n");
	for (int i = 0; i < hashtable_size; i++)
	{
		printf("Chain <%d> of Users:\n", i);
		user_t *access = user_hashtable_p[i];
		while (access != NULL)
		{
			printf("\t<%d>\n\t History Tree:\n", access->userID);
			printUserMovies(access->history);
			access = access->next;
		}
	}

	printf("Done\n");
	return 1;
}

//-----------------------------------------------------------------
// Prints all users' id, of the given chain
void printUsersChain(int j)
{
	printf("Chain <%d> of Users:\n", j);
	user_t *access = user_hashtable_p[j];
	while (access != NULL)
	{
		printf("\t<%d>\n", access->userID);
		access = access->next;
	}
}

void printCategory(int num)
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

// Inorder Traversal for New Releases Tree
void InOrderPrint(new_movie_t *p)
{
	if (p == NULL)
		return;
	InOrderPrint(p->lc);
	printf("<%d>, ", p->movieID);
	InOrderPrint(p->rc);
}

// initializes global variables
void initGlobal()
{
	srand(time(NULL));

	int i = 0;
	new_releases = NULL;
	movie_t *sen = (movie_t *)malloc(sizeof(movie_t));
	if (!sen)
	{
		printf("Malloc failed\n");
		exit(-1);
	}
	sen->movieID = -1;
	sen->year = 0;
	sen->watchedCounter = 0;
	sen->sumScore = 0;
	sen->lc = NULL;
	sen->rc = NULL;

	for (i = 0; i < NumOfCategor; i++)
	{
		categoryArray[i] = (movieCategory_t *)malloc(sizeof(movieCategory_t));
		categoryArray[i]->movie = NULL;
		categoryArray[i]->sentinel = sen;
	}

	int length = 170;
	// Alternatively
	//  length = sizeof(primes_g) / sizeof(primes_g[0]);
	while (i < length && primes_g[i] <= max_id)
	{
		i++;
	}
	if (i >= length || primes_g[i] <= max_id)
	{
		printf("No prime number greater than Users' max id was found\n");
		exit(-1);
	}
	p = primes_g[i];

	a = rand() % (p - 1) + 1; // aE{1,..,p-1}
	b = rand() % p;			  // bE{0,...,p-1}
	hashtable_size = max_users / 10;
	if (!hashtable_size)
	{
		hashtable_size = max_users;
	}
	user_hashtable_p = (user_t **)malloc(hashtable_size * sizeof(user_t *));
	if (!user_hashtable_p)
	{
		printf("Malloc failed\n");
		exit(-1);
	}
	for (i = 0; i < hashtable_size; i++)
	{
		user_hashtable_p[i] = (user_t *)malloc(sizeof(user_t));
		if (!user_hashtable_p[i])
		{
			printf("Malloc failed \n");
			exit(-1);
		}
		user_hashtable_p[i] = NULL;
	}
}

// Hash function
int hashFunction(int k) { return (((a * k + b) % p) % max_users); }

// Searches for the user with the given id
user_t *findUser(int userID)
{
	int indexHash = hashFunction(userID);
	if (indexHash < 0 || indexHash >= hashtable_size)
	{
		printf("User not found\n");
		return NULL;
	}
	user_t *UsersAccess = user_hashtable_p[indexHash];
	while (UsersAccess != NULL && UsersAccess->userID != userID)
	{
		UsersAccess = UsersAccess->next;
	}
	if (UsersAccess == NULL)
	{
		printf("User not found \n");
		return NULL;
	}
	return UsersAccess;
}

//------Distribute Event------------------------------------------

// counts how many movies of the given category there are
int movieCategorCounter(int categor, new_movie_t *p)
{
	if (p == NULL)
		return 0;
	if (p->category == categor)
	{
		return movieCategorCounter(categor, p->lc) + 1 + movieCategorCounter(categor, p->rc);
	}
	else
	{
		return movieCategorCounter(categor, p->lc) + movieCategorCounter(categor, p->rc);
	}
}

// converts a binary tree to a sorted array
void TreeToArray(int cat, int length, int *index, movie_t **array, new_movie_t *p)
{
	if (p == NULL || (*index) >= length)
	{
		return;
	}
	TreeToArray(cat, length, index, array, p->lc);
	if (cat == p->category)
	{
		movie_t *to_add = (movie_t *)malloc(sizeof(movie_t));
		if (!to_add)
		{
			printf("Malloc failed\n");
			return;
		}
		to_add->lc = categoryArray[cat]->sentinel;
		to_add->rc = categoryArray[cat]->sentinel;
		to_add->movieID = p->movieID;
		to_add->sumScore = p->sumScore;
		to_add->watchedCounter = p->watchedCounter;
		to_add->year = p->year;
		array[*index] = to_add;
		(*index)++;
	}
	TreeToArray(cat, length, index, array, p->rc);
}

// convers a sorted array to a binary search tree
movie_t *arrayToTree(movie_t **array, int n)
{
	movie_t *root = NULL;
	arrayBTS(0, n - 1, &root, array);
	printf("<%d>, ", root->movieID);

	return root;
}

void arrayBTS(int start, int end, movie_t **root, movie_t **array)
{
	if (start > end)
		return;
	int mid = (start + end) / 2;

	*root = BinInsert(root, array[mid]);
	arrayBTS(start, mid - 1, root, array);
	arrayBTS(mid + 1, end, root, array);
}

// Inserts nodes into the binary search tree
movie_t *BinInsert(movie_t **root, movie_t *node)
{
	if ((*root) == NULL)
	{
		(*root) = node;
	}
	else
	{

		int key = node->movieID;
		movie_t *access = (*root);
		movie_t *prev = NULL;
		while (access != NULL && access != categoryArray[0]->sentinel)
		{
			prev = access;
			if (key < access->movieID)
			{
				access = access->lc;
			}
			else
			{
				access = access->rc;
			}
		}

		if (key < prev->movieID)
		{
			prev->lc = node;
		}
		else
		{
			prev->rc = node;
		}
	}

	return *root;
}

// Inorder Traversal for each category Tree
void InOrderPrintCat(int i, movie_t *p)
{
	if (p == NULL || p == categoryArray[i]->sentinel)
		return;
	InOrderPrintCat(i, p->lc);
	printf("<%d>, ", p->movieID);
	InOrderPrintCat(i, p->rc);
}
//--------Watch Event------------------------------------------------
// Inserts nodes into History Tree of the given user
userMovie_t *HistoryInsert(user_t *UsersAccess, userMovie_t *to_add)
{
	userMovie_t *access = UsersAccess->history;
	if (access == NULL)
	{
		UsersAccess->history = to_add;
	}
	else
	{
		userMovie_t *prev = NULL;
		userMovie_t *node = (userMovie_t *)malloc(sizeof(userMovie_t));
		if (!node)
		{
			printf("Malloc failed \n");
			return NULL;
		}
		node->parent = NULL;
		node->lc = NULL;
		node->rc = NULL;
		node->category = -1;
		node->score = -1;
		to_add->parent = node;
		while (access != NULL)
		{
			prev = access;
			if (access->movieID > to_add->movieID)
			{
				access = access->lc;
			}
			else if (access->movieID <= to_add->movieID)
			{
				access = access->rc;
			}
		}
		if (prev->parent == NULL) // there is only one node
		{
			UsersAccess->history = node;
			prev->parent = node;
			if (prev->movieID < to_add->movieID)
			{
				node->movieID = to_add->movieID;
				node->rc = to_add;
				node->lc = prev;
			}
			else
			{
				node->movieID = prev->movieID;
				node->lc = to_add;
				node->rc = prev;
			}
		}
		else
		{
			if (prev->movieID < to_add->movieID)
			{
				node->movieID = to_add->movieID;
				node->rc = to_add;
				node->lc = prev;
				node->parent = prev->parent;
				if (prev->parent->movieID <= node->movieID) // right insert
				{
					prev->parent->rc = node;
				}
				else // left insert
				{
					prev->parent->lc = node;
				}
				prev->parent = node;
			}
			else
			{
				node->movieID = prev->movieID;
				node->lc = to_add;
				node->rc = prev;
				node->parent = prev->parent;
				if (prev->parent->movieID <= node->movieID) // right insert
				{
					prev->parent->rc = node;
				}
				else // left insert
				{
					prev->parent->lc = node;
				}
				prev->parent = node;
			}
		}
	}
	return UsersAccess->history;
}

// Inorder Traversal for the given History Tree
void printUserMovies(userMovie_t *root)
{
	if (root == NULL)
		return;
	printUserMovies(root->lc);
	if (root->lc == NULL && root->rc == NULL) // is leaf
	{
		printf("\t<%d, %d>\n", root->movieID, root->score);
	}

	printUserMovies(root->rc);
}
//-------Filter Movies-----------------------------------------------

// counts the number of the movies that have score greater or equal to the given one
void movieCount(int score, movie_t *root, int *sum)
{
	if (root == categoryArray[0]->sentinel) // its the same sentinel for all categories
		return;
	movieCount(score, root->lc, sum);
	if (root->watchedCounter > 0)
	{
		int average = (root->sumScore) / (root->watchedCounter);
		if (average >= score)
		{
			(*sum)++;
		}
	}
	movieCount(score, root->rc, sum);
}

// Stores the pointer of the movies with average score greater or equal to the given one
void findMovies(movie_t **array, int *index, int score, movie_t *root)
{
	if (root == categoryArray[0]->sentinel) // its the same sentinel for all categories
		return;
	findMovies(array, index, score, root->lc);
	if (root->watchedCounter > 0)
	{
		int average = AverageScore(root);
		if (average >= score)
		{
			array[(*index)] = root;
			(*index)++;
		}
	}
	findMovies(array, index, score, root->rc);
}

void InitialeHeap(movie_t **array, int n)
{
	for (int i = n / 2 - 1; i >= 0; i--)
	{
		Heapify(array, i, n);
	}
}

void Heapify(movie_t **array, int i, int n)
{
	int left = 2 * i + 1, right = 2 * i + 2, largest = i;

	if (left < n)
	{
		int averageLarge = AverageScore(array[largest]), averageLeft = AverageScore(array[left]);
		if (averageLeft > averageLarge)
		{
			largest = left;
		}
	}
	if (right < n)
	{
		int averageLarge = AverageScore(array[largest]), averageRight = AverageScore(array[right]);
		if (averageRight > averageLarge)
		{
			largest = right;
		}
	}
	if (largest != i)
	{
		swap(&array[i], &array[largest]);
		Heapify(array, largest, n);
	}
}

void HeapSort(movie_t **array, int n)
{
	InitialeHeap(array, n);

	for (int i = n - 1; i >= 0; i--)
	{
		swap(&array[0], &array[i]);
		Heapify(array, 0, i);
	}
}
void swap(movie_t **a, movie_t **b)
{

	movie_t *temp = *a;
	*a = *b;
	*b = temp;
}
int AverageScore(movie_t *node)
{

	return (node->sumScore / node->watchedCounter);
}
//------users’ average rate (Q)----------------------------
userMovie_t *FindNextLeaf(userMovie_t *node)
{
	if (node == NULL)
	{
		return NULL;
	}
	userMovie_t *par = node->parent;
	while (par != NULL)
	{
		if (par->movieID > node->movieID)
		{
			break;
		}
		par = par->parent;
	}
	if (!par)
	{
		return NULL;
	}
	par = par->rc;
	while (par != NULL)
	{
		if (par->lc == NULL && par->rc == NULL)
		{
			return par;
		}
		par = par->lc;
	}
	return NULL;
}
