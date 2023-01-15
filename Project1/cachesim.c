
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "cachesim.h"
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("Usage: %s <direct> <trace file name>\n", argv[0]);
        return 1;
    }

#ifdef DBG
    printf("BLOCK SIZE = %d Bytes\n", BLOCK_SIZE);
    printf("%d-WAY\n", WAY_SIZE);
    printf("CACHE SIZE = %d Bytes\n", CACHE_SIZE);
    printf("NUMBER OF BLOCKS = %d\n", NUM_BLOCKS);
    printf("NUMBER OF SETS = %d\n", NUM_SETS);
    printf("\n");
#endif

    char* trace_file_name = argv[2];
    struct direct_mapped_cache d_cache;
    char mem_request[20];
    uint64_t address;
    FILE *fp;


    /* Initialization */
    for (int i=0; i<NUM_BLOCKS; i++) {
        d_cache.valid_field[i] = 0;
        d_cache.dirty_field[i] = 0;
        d_cache.tag_field[i] = 0;
    }
    d_cache.hits = 0;
    d_cache.misses = 0;

    /* Opening the memory trace file */
    fp = fopen(trace_file_name, "r");

    if (strncmp(argv[1], "direct", 6)==0) { /* Simulating direct-mapped cache */
        /* Read the memory request address and access the cache */
        while (fgets(mem_request, 20, fp)!= NULL) {
            address = convert_address(mem_request);
            direct_mapped_cache_access(&d_cache, address);
        }

	//calculating hit rate and miss Rate for directed map
    float miss_rate = (float)(d_cache.misses)/(d_cache.hits + d_cache.misses); // Calculating miss rate
    float hit_rate = (float)(d_cache.hits)/(d_cache.hits + d_cache.misses); // calculating hit rate


        /*Print out the results*/
        printf("\n==================================\n");
        printf("Cache type:    Direct-Mapped Cache\n");
        printf("==================================\n");
        printf("Cache Hits:    %d\n", d_cache.hits);
        printf("Cache Misses:  %d\n", d_cache.misses);
	    printf("Total Memory Request: %d\n", d_cache.hits + d_cache.misses);
	    printf("Hit Rate : %.2f\n",miss_rate); //printing hit rate
	    printf("Miss Rate : %.2f\n",hit_rate); // printing miss rate
       	printf("\n");
    }


//For fully-associative we are using n-way simulator , we will just assign waysize equals to number of blocks as per the definition of fully associative cache
  else if (strncmp(argv[1], "fully-associative", strlen("fully-associative")) == 0)
  {
    WAY_SIZE = NUM_BLOCKS; //  assigned waysize equals to number of blocks
    NUM_SETS = NUM_BLOCKS / WAY_SIZE; // Number of sets would be 1 in this case
    struct n_way_set_associative_cache n_cache;
 
    /* Initialization */
    for (int i=0; i<NUM_BLOCKS; i++)
    {
      n_cache.valid_field[i] = 0;
      n_cache.dirty_field[i] = 0;
      n_cache.tag_field[i] = 0;
      n_cache.index_field[i] = 0;
    }
    n_cache.hits = 0;
    n_cache.misses = 0;

    /* Simulating fully-associative n_cache */
    while (fgets(mem_request, 20, fp) != NULL)
    {
      address = convert_address(mem_request);
      n_way_set_associative_cache_access(&n_cache, address, WAY_SIZE); // Calling n-way set associative function with fully associative arguments
    }

    //calculating hit and miss Rate
    double cache_miss_rate = (float)n_cache.misses / (n_cache.hits + n_cache.misses);
    double cache_hit_rate = (float)n_cache.hits / (n_cache.hits + n_cache.misses);

    /*Print out the results*/
    printf("\n==================================\n");
    printf("Cache type:    Fully-Associative Cache\n");
    printf("==================================\n");
    printf("Cache Hits:       %d\n", n_cache.hits);
    printf("Cache Misses:     %d\n", n_cache.misses);
    printf("Total Memory Request: %d\n", n_cache.hits + n_cache.misses);
    printf("Cache Miss Rate:  %.2f\n", cache_miss_rate);
    printf("Cache Hit Rate:   %.2f\n", cache_hit_rate);
    printf("\n");
  }

  // For n-way associative , initializing variables and calling n-way simulator function ,If 1st argument is "n-way" then it will be executed
  else if(strncmp(argv[1], "n-way", strlen("n-way")) == 0)
  {
    WAY_SIZE = atoi(argv[3]); // We are taking way size from 3rd argument
    NUM_SETS = NUM_BLOCKS / WAY_SIZE; //General formulae to calculate number of sets

    struct n_way_set_associative_cache n_cache; // defining variable ncache for later use(taking reference of header file)

    /* Initialization of all fields to zero like valid ,dirty , and tag field for all blocks*/
    for (int i=0; i<NUM_BLOCKS; i++)
    {
      n_cache.valid_field[i] = 0;
      n_cache.dirty_field[i] = 0;
      n_cache.tag_field[i] = 0;
      n_cache.index_field[i] = 0;
    }
    n_cache.hits = 0;
    n_cache.misses = 0;

    /* Simulating n-way associative cache */
    while (fgets(mem_request, 20, fp) != NULL)
    {
      address = convert_address(mem_request); 
      n_way_set_associative_cache_access(&n_cache, address, WAY_SIZE); //calling n-way simulator function with required values
    }

    //calculainge hit rate and miss Rate
    double cache_miss_rate = (float)n_cache.misses / (n_cache.hits + n_cache.misses); // calculating hit rate 
    double cache_hit_rate = (float)n_cache.hits / (n_cache.hits + n_cache.misses); // calculating miss rate

    /*Print out the results*/
    printf("\n==================================\n");
    printf("Cache type:    %d-Associative Cache\n", WAY_SIZE);
    printf("==================================\n");
    printf("Cache Hits:       %d\n", n_cache.hits);
    printf("Cache Misses:     %d\n", n_cache.misses);
    printf("Total Memory Request: %d\n", n_cache.hits + n_cache.misses);    
    printf("Cache Miss Rate:  %.2f\n", cache_miss_rate);
    printf("Cache Hit Rate:   %.2f\n", cache_hit_rate);
    printf("\n");
  }


 //For 2-level cache associative , if 1st argument is "2-level" then it will be executed
  else if(strncmp(argv[1], "2-level", strlen("2-level")) == 0)
  {
    //Defined separate variables for each cache level to remove confusion
    // Block and way size is defined in header file as it is given in requirment ,so hard coded the value in header file
    L1_NUM_SETS = L1_NUM_BLOCKS / L1_WAY_SIZE; // Number of sets for L1 cache
    L2_NUM_SETS = L2_NUM_BLOCKS / L2_WAY_SIZE; // Number of sets for L2 cache

    struct level1_set_associative_cache l1_cache; // Using level 1 structure from header file as l1_cache for later use
    struct level2_set_associative_cache l2_cache; //Using level 2 structure from header file as l1_cache for later use

    /* Initialization  of all fields of level1 and level 2 cache for all blocks*/
    for (int i=0; i<L1_NUM_BLOCKS; i++)
    {
      l1_cache.l1_valid_field[i] = 0;
      l1_cache.l1_dirty_field[i] = 0;
      l1_cache.l1_tag_field[i] = 0;
      l1_cache.l1_index_field[i] = 0;
    }
    for(int i=0; i<L2_NUM_BLOCKS; i++){ 

      l2_cache.l2_valid_field[i] = 0;
      l2_cache.l2_dirty_field[i] = 0;
      l2_cache.l2_tag_field[i] = 0;
      l2_cache.l2_index_field[i] = 0;

    }
    
    //Initializing number of hits and misses to zero for both levels
    l1_cache.hits = 0;
    l1_cache.misses = 0;
    l2_cache.hits = 0;
    l2_cache.misses = 0;

    /* Simulating n-way associative n_cache */
    int total_memory_accesses = 0;
    while (fgets(mem_request, 20, fp) != NULL)
    {
      address = convert_address(mem_request);
      level2_cache_access(&l1_cache,&l2_cache, address);
      total_memory_accesses++;
    }

    //calculainge hit rate and miss Rate for both L1 and L2 cache
    double l1_cache_miss_rate = (float)l1_cache.misses / total_memory_accesses;
    double l1_cache_hit_rate = (float)l1_cache.hits / total_memory_accesses;
    double l2_cache_miss_rate = (float)l2_cache.misses / (l2_cache.hits + l2_cache.misses);
    double l2_cache_hit_rate = (float)l2_cache.hits / (l2_cache.hits + l2_cache.misses);

    /*Print out the results*/
    printf("\n==================================\n");
    printf("Cache type:    2-level-set-Associative Cache\n");
    printf("==================================\n");
    printf("L1 Cache Hits:       %d\n", l1_cache.hits);
    printf("L1 Cache Misses:     %d\n", l1_cache.misses);
    printf("L1 Cache Miss Rate:  %.2f\n", l1_cache_miss_rate);
    printf("L1 Cache Hit Rate:   %.2f\n", l1_cache_hit_rate);
    printf("L2 Cache Hits:       %d\n", l2_cache.hits);
    printf("L2 Cache Misses:     %d\n", l2_cache.misses);
    printf("L2 Cache Miss Rate:  %.2f\n", l2_cache_miss_rate);
    printf("L2 Cache Hit Rate:   %.2f\n", l2_cache_hit_rate);
    printf("Total Memory Request:   %d\n", total_memory_accesses);
    printf("\n");
  }

    fclose(fp);

    return 0;
}

uint64_t convert_address(char memory_addr[])
/* Converts the physical 32-bit address in the trace file to the "binary" \\
 * (a uint64 that can have bitwise operations on it) */
{
    uint64_t binary = 0;
    int i = 0;

    while (memory_addr[i] != '\n') {
        if (memory_addr[i] <= '9' && memory_addr[i] >= '0') {
            binary = (binary*16) + (memory_addr[i] - '0');
        } else {
            if(memory_addr[i] == 'a' || memory_addr[i] == 'A') {
                binary = (binary*16) + 10;
            }
            if(memory_addr[i] == 'b' || memory_addr[i] == 'B') {
                binary = (binary*16) + 11;
            }
            if(memory_addr[i] == 'c' || memory_addr[i] == 'C') {
                binary = (binary*16) + 12;
            }
            if(memory_addr[i] == 'd' || memory_addr[i] == 'D') {
                binary = (binary*16) + 13;
            }
            if(memory_addr[i] == 'e' || memory_addr[i] == 'E') {
                binary = (binary*16) + 14;
            }
            if(memory_addr[i] == 'f' || memory_addr[i] == 'F') {
                binary = (binary*16) + 15;
            }
        }
        i++;
    }

#ifdef DBG
    printf("%s converted to %llu\n", memory_addr, binary);
#endif
    return binary;
}

void direct_mapped_cache_access(struct direct_mapped_cache *cache, uint64_t address)
{
    uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE);
    uint64_t index = block_addr % NUM_BLOCKS;
    uint64_t tag = block_addr >> (unsigned)log2(NUM_BLOCKS);

#ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu ", address, block_addr, index, tag);
#endif

    if (cache->valid_field[index] && cache->tag_field[index] == tag) { /* Cache hit */
        cache->hits += 1;
#ifdef DBG
        printf("Hit!\n");
#endif
    } else {
        /* Cache miss */
        cache->misses += 1;
#ifdef DBG
        printf("Miss!\n");
#endif
        if (cache->valid_field[index] && cache->dirty_field[index]) {
            /* Write the cache block back to memory */
        }
        cache->tag_field[index] = tag;
        cache->valid_field[index] = 1;
        cache->dirty_field[index] = 0;
    }
    
}

//Writing function for n-way associative cache simulator 
void n_way_set_associative_cache_access(struct n_way_set_associative_cache *cache, uint64_t address, int ways)
{  
  uint64_t block_addr = address >> (unsigned)log2(BLOCK_SIZE); //calculating block address from block size
  uint64_t set = block_addr % NUM_SETS; //selects set index
  uint64_t block_index = set * ways; //selects cache block index 
  uint64_t tag = block_addr >> (unsigned)log2(NUM_BLOCKS); // selecting tag value from numer of blocks
  bool hit = false; // flag variable to check hit or miss

  #ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu\n", address, block_addr, set, tag);
  #endif

  //traversing whole set to check if any hit found
  for (int i = 0; i < ways; i++)
  {
    if (cache->tag_field[block_index] == tag &&  cache->valid_field[block_index])
    {
      //If it satisfies then increment number of cache hits
      cache->hits += 1;
      hit = true;
      #ifdef DBG
        printf("Hit!\n");
      #endif
      break;
    }
    block_index++;
  }

  //Check if is hit or miss
  //If there is a miss in cache then search for valid field and apply random replacement algorithm to replace data in cache from main memory
  if (!hit) 
  {     
    /* Cache miss */
    cache->misses += 1;
    #ifdef DBG
      printf("Miss!\n");
    #endif
    bool valid_field_found = false;
    block_index  = set * ways; //set block index to the first value of set
   
    //check if any valid bits found 
    for (int i = 0; i < ways; i++)
    {
      if (cache->valid_field[block_index] == 0)
      {
        cache->tag_field[block_index] = tag; // Assigning tag field
        cache->valid_field[block_index] = 1;
        cache->dirty_field[block_index] = 0;
        valid_field_found = true;
        break;
      }
      block_index ++;
    }

    //If no valid field found , then select index randomly and assign value in it
    if (!valid_field_found)
    {
      time_t t;
      srand((unsigned) time(&t));// using srand function to generate random number  
      int random = rand() % ways; //return random numbers from 0 to n-1 ways
      block_index = set * ways; //setting block index to start
      block_index = block_index + random; //setting block index to random position in set

      cache->tag_field[block_index] = tag; // Assign Tag field
      cache->valid_field[block_index] = 1;
      cache->dirty_field[block_index] = 0;


        #ifdef DBG
            printf("block replaced at block_index = %llu, set = %llu\n", block_index, set);
        #endif

    }

    /* Write back to memory if needed before overwriting */
    if (cache->valid_field[block_index] && cache->dirty_field[block_index])
    {
        /* Write the n_cache block back to  memory  */
    }
  }
}

// For 2 level cache if there is a miss in L1 then ,it searches for same in L2 , and If there is a hit in L2 cache then it replaces block in L1 or if miss then it further checks in main memory and replace the block in L2 Cache

void level2_cache_access(struct level1_set_associative_cache *l1_cache,struct level2_set_associative_cache *l2_cache, uint64_t address)
{
  uint64_t l1_block_addr = address >> (unsigned)log2(L1_BLOCK_SIZE); //calculating block address from block size
  uint64_t l1_set = l1_block_addr % L1_NUM_SETS; //selecting set index
  uint64_t l1_block_iterator = l1_set * L1_WAY_SIZE; //selecting cache block index 
  uint64_t l1_tag = l1_block_addr >> (unsigned)log2(L1_NUM_BLOCKS);
  bool l1_hit = false;

  #ifdef DBG
    printf("Memory address: %llu, Block address: %llu, Index: %llu, Tag: %llu\n", address, block_addr, set_index, tag);
  #endif

  //traversing whole L1 set to search for hits
  for (int i = 0; i < L1_WAY_SIZE; i++)
  {
     //check if it is hit
    if (l1_cache->l1_valid_field[l1_block_iterator] && l1_cache->l1_tag_field[l1_block_iterator] == l1_tag)
    {
     
      l1_cache->hits += 1; //increment number of L1 hits
      l1_hit = true;
      
      #ifdef DBG
        printf("Hit!\n");
      #endif
      break;
    }
    l1_block_iterator++;
  }

  //Check if it is hit or miss in L1
  if (!l1_hit)
  {
    //It is a Cache miss in L1
    l1_cache->misses += 1;
    #ifdef DBG
      printf("Miss!\n");
    #endif


  uint64_t l2_block_addr = address >> (unsigned)log2(L2_BLOCK_SIZE); //the block address of the memory byte address
  uint64_t l2_set_index = l2_block_addr % L2_NUM_SETS; //selects set
  uint64_t l2_block_iterator = l2_set_index * L2_WAY_SIZE; //selects cache block index 
  uint64_t l2_tag = l2_block_addr >> (unsigned)log2(L2_NUM_BLOCKS);
  bool l2_hit = false;

  //If it is a miss in L1 , then search in L2 cache
  for (int i = 0; i < L2_WAY_SIZE; i++)
  {
    if (l2_cache->l2_valid_field[l2_block_iterator] && l2_cache->l2_tag_field[l2_block_iterator] == l2_tag)
    {
      //Its a cache in hit in L2
      l2_cache->hits += 1;
      l2_hit = true;
      #ifdef DBG
        printf("Hit!\n");
      #endif
      break;
    }
    l2_block_iterator++;
  }
  
 // If there is a cache hit in L2 , then replace the block in L1 using random replacement
if (l2_hit)
 {
 
l1_block_iterator = l1_set * L1_WAY_SIZE;
bool l1_valid_found = false;

 //check if any valid bit exist
    for (int i = 0; i < L1_WAY_SIZE; i++)
    {
      if (l1_cache->l1_valid_field[l1_block_iterator] == 0)
      {
        l1_cache->l1_tag_field[l1_block_iterator] = l1_tag;
        l1_cache->l1_valid_field[l1_block_iterator] = 1;
        l1_cache->l1_dirty_field[l1_block_iterator] = 0;

        l1_valid_found = true;
        break;
      }
      l1_block_iterator++;
    }

        //If valid bit not exist then replace randomly
    if (!l1_valid_found)
    {
      // choose a block at random to replace
      time_t t;
      srand((unsigned) time(&t)); //using srand function to generate random number
      int random_number = rand() % L1_WAY_SIZE; //return random number from 0 to n-1 ways
      l1_block_iterator = l1_set * L1_WAY_SIZE; //setting block index to start
      l1_block_iterator = l1_block_iterator + random_number; //setting block index to random position in set

      l1_cache->l1_tag_field[l1_block_iterator] = l1_tag; //Assign L1 tag value
      l1_cache->l1_valid_field[l1_block_iterator] = 1;
      l1_cache->l1_dirty_field[l1_block_iterator] = 0;


        #ifdef DBG
            printf("block replaced at block_index = %llu, set_index = %llu\n", block_iterator, set_index);
        #endif

    }


  }
  // If there is also a miss in L2, Then replace the block in L2 cache randomly
  else if(!l2_hit){

l2_cache -> misses += 1; // Increment miss count
l2_block_iterator = l2_set_index * L2_WAY_SIZE; //set block index
int l2_valid_found = 0;

 //check for any valid bits == 0
    for (int i = 0; i < L2_WAY_SIZE; i++)
    {
      if (l2_cache->l2_valid_field[l2_block_iterator] == 0)
      {
        l2_cache->l2_tag_field[l2_block_iterator] = l2_tag; // Assign L2 tag value
        l2_cache->l2_valid_field[l2_block_iterator] = 1;
        l2_cache->l2_dirty_field[l2_block_iterator] = 0;

        l2_valid_found = 1;
        break;
      }
      l1_block_iterator++;
    }

        //check if replaced invalid block
    if (!l2_valid_found)
    {
      //invalid block was not found to replace, choose a block at random
      time_t t;
      srand((unsigned) time(&t));
      int random_number = rand() % L2_WAY_SIZE; //return random number from 0 to n-1

      l2_block_iterator = l2_set_index * L2_WAY_SIZE; //set block index to beginning of set
      l2_block_iterator = l2_block_iterator + random_number; //set block index to random position within set, that is the one we replace

      l2_cache->l2_tag_field[l2_block_iterator] = l2_tag; //Assign L2 tag value
      l2_cache->l2_valid_field[l2_block_iterator] = 1;
      l2_cache->l2_dirty_field[l2_block_iterator] = 0;

        #ifdef DBG
            printf("block replaced at block_index = %llu, set_index = %llu\n", block_iterator, set_index);
        #endif

    }




  }   
}  
}


// End of Code