#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>

struct list_node_s {
	int data;
	struct list_node_s *next;
};

const int MAX_THREADS = 1024;
long thread_count;

pthread_mutex_t list_mutex;
pthread_mutex_t member_mutex;
pthread_mutex_t insert_mutex;
pthread_mutex_t delete_mutex;
pthread_mutex_t total_mutex;

struct list_node_s* head = NULL;

int n, m;
float m_insert_fraction, m_delete_fraction, m_member_fraction;
float m_insert = 0.0, m_delete = 0.0, m_member = 0.0;
int memberCount = 0, insertCount = 0, deleteCount = 0;

int Member(int value, struct list_node_s *head_p);

int Insert(int value, struct list_node_s **head_pp);

int Delete(int value, struct list_node_s **head_pp);

void *Thread_Operation();

void getInputs(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	long thread;
	pthread_t* thread_handles;
	struct timeval startTime, endTime;
	double elapsedTime;

	getInputs(argc, argv);

	srand(time(NULL));
	for (int i = 0; i < n;) {
		int r = rand() % 65535;
		i += Insert(r, &head);
	}

	m_insert = m_insert_fraction * m;
	m_delete = m_delete_fraction * m;
	m_member = m_member_fraction * m;

	thread_handles = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
	FILE *fp;
	fp = fopen("OneMutexResults.txt", "a");

	gettimeofday(&startTime, NULL);
	pthread_mutex_init(&list_mutex, NULL);
	pthread_mutex_init(&member_mutex, NULL);
	pthread_mutex_init(&insert_mutex, NULL);
	pthread_mutex_init(&delete_mutex, NULL);
	pthread_mutex_init(&total_mutex, NULL);

	for (thread = 0; thread < thread_count; thread++) {
		pthread_create(&thread_handles[thread], NULL, Thread_Operation, NULL);
	}

	for (thread = 0; thread < thread_count; thread++) {
		pthread_join(thread_handles[thread], NULL);
	}

	gettimeofday(&endTime, NULL);

	// Destroying the mutex
	pthread_mutex_destroy(&list_mutex);
	pthread_mutex_destroy(&member_mutex);
	pthread_mutex_destroy(&insert_mutex);
	pthread_mutex_destroy(&delete_mutex);
	pthread_mutex_destroy(&total_mutex);

	elapsedTime = (double) (endTime.tv_usec - startTime.tv_usec) / 1000000
			+ (double) (endTime.tv_sec - startTime.tv_sec);

fprintf(fp, "%.6f\n", elapsedTime);

	fclose(fp);

	return 0;
}

int Member(int value, struct list_node_s *head_p) {
	struct list_node_s *curr_p = head_p;

	while (curr_p != NULL && curr_p->data < value)
		curr_p = curr_p->next;

	if (curr_p == NULL || curr_p->data > value) {
		return 0;
	} else {
		return 1;
	}
}

int Insert(int value, struct list_node_s **head_pp) {
	struct list_node_s *curr_p = *head_pp;
	struct list_node_s *pred_p = NULL;
	struct list_node_s *temp_p;

	while (curr_p != NULL && curr_p->data < value) {
		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	if (curr_p == NULL || curr_p->data > value) {
		temp_p = malloc(sizeof(struct list_node_s));
		temp_p->data = value;
		temp_p->next = curr_p;
		if (pred_p == NULL)
			*head_pp = temp_p;
		else
			pred_p->next = temp_p;
		return 1;
	} else {
		return 0;
	}
}

int Delete(int value, struct list_node_s **head_pp) {
	struct list_node_s *curr_p = *head_pp;
	struct list_node_s *pred_p = NULL;

	while (curr_p != NULL && curr_p->data < value) {
		pred_p = curr_p;
		curr_p = curr_p->next;
	}

	if (curr_p != NULL && curr_p->data == value) {
		if (pred_p == NULL) {
			*head_pp = curr_p->next;
			free(curr_p);
		} else {
			pred_p->next = curr_p->next;
			free(curr_p);
		}
		return 1;
	} else {
		return 0;
	}
}

int totalCount = 0;

int finished_member = 0;
int finished_insert = 0;
int finished_delete = 0;

void *Thread_Operation() {

	while (totalCount < m) {

		// Variable to randomly generate values for operations
		int random_value = rand() % 65535;

		// Variable to randomly select one of the three operations
		int random_select = rand() % 3;

		// Member operation
		if (random_select == 0) {
			pthread_mutex_lock(&member_mutex);
			if (finished_member == 0) {
				if (memberCount < m_member) {
					memberCount++;
					pthread_mutex_unlock(&member_mutex);
					pthread_mutex_lock(&total_mutex);
					totalCount++;
					pthread_mutex_unlock(&total_mutex);
					pthread_mutex_lock(&list_mutex);
					Member(random_value, head);
					pthread_mutex_unlock(&list_mutex);
				} else {
					finished_member = 1;
					pthread_mutex_unlock(&member_mutex);
				}
			} else {
				pthread_mutex_unlock(&member_mutex);
			}

		}

		else if (random_select == 1) {
			pthread_mutex_lock(&insert_mutex);
			if (finished_insert == 0) {
				if (insertCount < m_insert) {
					insertCount++;
					pthread_mutex_unlock(&insert_mutex);
					pthread_mutex_lock(&total_mutex);
					totalCount++;
					pthread_mutex_unlock(&total_mutex);
					pthread_mutex_lock(&list_mutex);
					Insert(random_value, &head);
					pthread_mutex_unlock(&list_mutex);
				} else {
					finished_insert = 1;
					pthread_mutex_unlock(&insert_mutex);
				}
			} else {
				pthread_mutex_unlock(&insert_mutex);
			}

		}

		else if (random_select == 2) {
			pthread_mutex_lock(&delete_mutex);
			if (finished_delete == 0) {
				if (deleteCount < m_delete) {
					deleteCount++;
					pthread_mutex_unlock(&delete_mutex);
					pthread_mutex_lock(&total_mutex);
					totalCount++;
					pthread_mutex_unlock(&total_mutex);
					pthread_mutex_lock(&list_mutex);
					Delete(random_value, &head);
					pthread_mutex_unlock(&list_mutex);
				} else {
					finished_delete = 1;
					pthread_mutex_unlock(&delete_mutex);
				}
			} else {
				pthread_mutex_unlock(&delete_mutex);
			}
		}
	}
	return NULL;
}

void getInputs(int argc, char *argv[]) {
	if (argc != 7) {
		printf(
				"Please give the command in this format : ./mutexLinkedList <n> <m> <thread_count> <mMember> <mInsert> <mDelete>\n");
		exit(0);
	}

	n = (int) strtol(argv[1], (char **) NULL, 10);
	m = (int) strtol(argv[2], (char **) NULL, 10);
	thread_count = (int) strtol(argv[3], (char **) NULL, 10);

	m_member_fraction = (float) atof(argv[4]);
	m_insert_fraction = (float) atof(argv[5]);
	m_delete_fraction = (float) atof(argv[6]);

	if (thread_count <= 0 || thread_count > MAX_THREADS) {
		printf(
				"Please give provide a valid number of threads in the range of 0 to %d\n",
				MAX_THREADS);
		exit(0);
	}
	//Validating the arguments
	if (n <= 0 || m <= 0
			|| m_member_fraction + m_insert_fraction + m_delete_fraction
					!= 1.0) {
		printf(
				"Please give the command with the arguments: ./mutexLinkedList <n> <m> <thread_count> <mMember> <mInsert> <mDelete>\n");

		if (n <= 0)
			printf(
					"Please provide a valid number of nodes for the linked list (value of n)\n");

		if (m <= 0)
			printf(
					"Please provide a valid number of operations for the linked list (value of m)\n");

		if (m_member_fraction + m_insert_fraction + m_delete_fraction != 1.0)
			printf(
					"Please provide valid fractions of operations for the linked list (value of mMember, mInsert, mDelete)\n");

		exit(0);
	}
}

