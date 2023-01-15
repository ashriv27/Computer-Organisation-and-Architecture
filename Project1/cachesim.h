
#define BLOCK_SIZE  16        /* Cache block size (or cache line size) in bytes \\
                                 (must be power of 2). 4 Bytes = 1 Word */

//#define BLOCK_SIZE  32   // Use this For 32 bytes cache line size        
//#define BLOCK_SIZE  128  // Use this for 128 bytes cache line size

//#define WAY_SIZE 1       /* Associativity; 1-way = direct-mapped */
#define CACHE_SIZE  32768     /* Cache capacity in bytes (must be power of 2)*/ // for 32 Kb cache size
//#define CACHE_SIZE  16384  // Use this for 16 KB cache size
//#define CACHE_SIZE  65536  // Use this for 64 KB cache size

#define NUM_BLOCKS  (CACHE_SIZE / BLOCK_SIZE)

int WAY_SIZE;  // Defining Way Size as integer , It value will be taken from command line arguments
int NUM_SETS = 0; // Defining number of sets as integer (NUM_BLOCKS / WAY_SIZE)

int L1_NUM_SETS = 0; // It is defined for level 1 cache 
int L2_NUM_SETS = 0;// It is defined for level 2 cache

//#define DBG
#define _CRT_SECURE_NO_WARNINGS

//Separate variables for level 1 and level 2 cache
#define L1_BLOCK_SIZE  64
#define L2_BLOCK_SIZE  64
int L1_WAY_SIZE = 2;
int L2_WAY_SIZE = 8;
#define L1_CACHE_SIZE  65536
#define L2_CACHE_SIZE  1048576
#define L1_NUM_BLOCKS  (L1_CACHE_SIZE / L1_BLOCK_SIZE)
#define L2_NUM_BLOCKS  (L2_CACHE_SIZE / L2_BLOCK_SIZE)


/*The data structure of direct-mapped cache*/
struct direct_mapped_cache {
    unsigned valid_field[NUM_BLOCKS];   /* Valid field */
    unsigned dirty_field[NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_BLOCKS];     /* Tag field */
    char data_field[NUM_BLOCKS][BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
};

/*The data structure of n-way set associate cache*/
struct n_way_set_associative_cache {
    unsigned valid_field[NUM_BLOCKS];   /* Valid field */
    unsigned dirty_field[NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t tag_field[NUM_BLOCKS];     /* Tag field */
    uint64_t index_field[NUM_BLOCKS];   /* index field used to identify set */
    char data_field[NUM_BLOCKS][BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
};

/*The data structure of level-1 cache*/
struct level1_set_associative_cache {
    
    unsigned l1_valid_field[L1_NUM_BLOCKS];   /* Valid field */
    unsigned l1_dirty_field[L1_NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t l1_tag_field[L1_NUM_BLOCKS];     /* Tag field */
    uint64_t l1_index_field[L1_NUM_BLOCKS];   /* index field used to identify set */
    char l1_data_field[L1_NUM_BLOCKS][L1_BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
};


/*The data structure of level-2 cache*/
struct level2_set_associative_cache {
    unsigned l2_valid_field[L2_NUM_BLOCKS];   /* Valid field */
    unsigned l2_dirty_field[L2_NUM_BLOCKS];   /* Dirty field; since we don't distinguish writes and \\
                                           reads in this project yet, this field doesn't really matter */
    uint64_t l2_tag_field[L2_NUM_BLOCKS];     /* Tag field */
    uint64_t l2_index_field[L2_NUM_BLOCKS];   /* index field used to identify set */
    char l2_data_field[L2_NUM_BLOCKS][L2_BLOCK_SIZE];  /* Data field; since we don't really fetch data, \\
                                                 this field doesn't really matter */
    int hits;                          /* Hit count */
    int misses;                        /* Miss count */
};



/*Read the memory traces and convert it to binary*/
uint64_t convert_address(char memory[]);

/*Simulate the direct-mapped cache*/
void direct_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address);

/*Simulate the n-way set associative cache*/
void n_way_set_associative_cache_access(struct n_way_set_associative_cache *cache, uint64_t address, int n);

/*Simulate the 2 level set associative cache*/
void level2_cache_access(struct level1_set_associative_cache *l1_cache,struct level2_set_associative_cache *l2_cache, uint64_t address);