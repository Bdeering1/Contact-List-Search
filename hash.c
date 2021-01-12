#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <ctype.h>

#include "a4.h"

#define BUFFER_SIZE	1024

int collisions;
int duplicates;

#ifdef PARSE
int char_frequency[27];
int length_frequency[64];
#endif

struct record /* DO NOT INCLUDE */
{
  char *last_name; /* original */
  char *first_name; /* original */
  char *license_no;
  char *license_type; /* original */
  char *issue_date;
};

struct array /* DO NOT INCLUDE */
{
  struct record *arr;
  int nelements;
  struct record **hash;
  int hash_size;
};

int char2int( unsigned char c ) /* DO NOT INCLUDE */
{
  if ( isupper(c) )
  {
    return (int)(c - 'A');
  }
  if ( islower(c) )
  {
    return (int)(c - 'a');
  }
  return 26;
}

int str2int( char *s, int max ) /* DO NOT INCLUDE */
{
  char *c;
  unsigned long number, column, new;

  column = 1;
  number = 0;
  for (c=s;(*c);c++)
  {
    number += char2int(*c) * column;
    column *= 27;
  }

  new = 0;
  while (number)
  {
    new = ( number + (new % max) ) % max;
    number = number / max;
  }
  
  return (int)new;
}

struct array *read_records() /* DO NOT INCLUDE */
{
  char buffer[BUFFER_SIZE];

  struct array *arrptr;

  FILE *fp;

  int line, start, end;
  
  #ifdef PARSE
  int i;
  for (i = 0; i < sizeof(char_frequency) / sizeof(int); i++) {
    char_frequency[i] = 0;
  }
  #endif

  arrptr = malloc( sizeof( struct array ) );
  arrptr->nelements = 0;

  fp = fopen( "Professional_and_Occupational_Licensing.csv", "r" );
  fgets( buffer, BUFFER_SIZE, fp );

  while (!feof(fp)) /*** finding number of elements in array */
  {
    if ( fgets( buffer, BUFFER_SIZE, fp )==NULL )
    {
      break;
    }

    if ( strlen(buffer)==BUFFER_SIZE-1 )
    {
      fprintf( stderr, "Error:  BUFFER TOO SMALL\n" );
      exit(-1);
    }

    (arrptr->nelements)++;
  }

  arrptr->arr = malloc( sizeof( struct record ) * (arrptr->nelements) ); /*** allocating array */

  fseek( fp, 0, SEEK_SET );

  fgets( buffer, BUFFER_SIZE, fp );
  for (line=0;line<arrptr->nelements;line++)
  {
    fgets( buffer, BUFFER_SIZE, fp );

    start = 0;
    for (end=start;buffer[end]!=',';end++);	/* find next comma */

    (arrptr->arr)[line].last_name = malloc( end - start + 1 ); /*** finding last_name */
    strncpy( (arrptr->arr)[line].last_name, buffer+start, end-start );
    (arrptr->arr)[line].last_name[end-start] = '\0';
    
    #ifdef PARSE
    for (i = 0; i < end - start; i++)
      char_frequency[char2int((arrptr->arr)[line].last_name[end-start-(i+1)])]++;
    
    length_frequency[end - start]++;
    #endif

    start = end+1;
    for (end=start;buffer[end]!=',';end++); /* find next comma */

    (arrptr->arr)[line].first_name = malloc( end - start + 1 ); /*** finding first_name */
    strncpy( (arrptr->arr)[line].first_name, buffer+start, end-start );
    (arrptr->arr)[line].first_name[end-start] = '\0';


    start = end+1;
    for (end=start;buffer[end]!=',';end++); /* find next comma */

    start = end+1;
    for (end=start;buffer[end]!=',';end++); /* find next comma */

    start = end+1;
    for (end=start;buffer[end]!=',';end++); /* find next comma */
    
    (arrptr->arr)[line].license_no = malloc( end - start + 1 ); /*** finding license_no */
    strncpy( (arrptr->arr)[line].license_no, buffer+start, end-start );
    (arrptr->arr)[line].license_no[end-start] = '\0';

    
    start = end+1;
    for (end=start;buffer[end]!=',';end++); /* find next comma */

    start = end+1;
    for (end=start;buffer[end]!=',';end++); /* find next comma */

    (arrptr->arr)[line].license_type = malloc( end - start + 1 ); /*** finding license_type */
    strncpy( (arrptr->arr)[line].license_type, buffer+start, end-start );
    (arrptr->arr)[line].license_type[end-start] = '\0';
    
    
    start = end+1;
    for (end=start;buffer[end]!=',';end++); /* find next comma */
    
    start = end+1;
    for (end=start;buffer[end]!=',';end++); /* find next comma */
    
    start = end+1;
    for (end=start;buffer[end]!=',';end++); /* find next comma */
    
    start = end+1;
    for (end=start;buffer[end]!=',';end++); /* find next comma */
    
    start = end+1;
    for (end=start;buffer[end]!=',';end++); /* find next comma */
    
    (arrptr->arr)[line].issue_date = malloc( end - start + 1 ); /*** finding issue_date */
    strncpy( (arrptr->arr)[line].issue_date, buffer+start, end-start );
    (arrptr->arr)[line].issue_date[end-start] = '\0';
    
  }


  return arrptr;
}


void build_hash( struct array *arrptr, int hash_size ) /* DO NOT INCLUDE */
{
  int idx, line; /*** idx = bucket */
  int duplicate;

  arrptr->hash_size = hash_size;
  arrptr->hash = malloc( sizeof( struct record * ) * arrptr->hash_size ); /*** allocating hash */

  for (idx=0;idx<arrptr->hash_size;idx++) /*** setting each record pointer to NULL to begin */
  {
    (arrptr->hash)[idx] = NULL;
  }

  for (line=0;line<arrptr->nelements;line++)
  {
    
    #ifdef HASH0
    printf( "%d Adding %s\n", line, (arrptr->arr)[line].last_name );
    idx = str2int( (arrptr->arr)[line].last_name, arrptr->hash_size );
    #endif
    #ifdef HASH1
    printf( "%d Adding %s\n", line, (arrptr->arr)[line].last_name );
    idx = hash1( (arrptr->arr)[line].last_name, arrptr->hash_size );
    #endif
    #ifdef HASH2
    printf( "%d Adding %s\n", line, (arrptr->arr)[line].license_no );
    idx = hash2( (arrptr->arr)[line].license_no, arrptr->hash_size );
    #endif
    #ifdef HASH3
    printf( "%d Adding %s\n", line, (arrptr->arr)[line].issue_date );
    idx = hash3( (arrptr->arr)[line].issue_date, arrptr->hash_size );
    #endif

    duplicate = 0;
    while ( (arrptr->hash)[idx]!=NULL ) /*** while the bucket is not empty */
    {
      
      #ifdef HASH0
      duplicate = strcmp( ((arrptr->hash)[idx])->last_name, (arrptr->arr)[line].last_name );
      #endif
      #ifdef HASH1
      duplicate = strcmp( ((arrptr->hash)[idx])->last_name, (arrptr->arr)[line].last_name );
      #endif
      #ifdef HASH2
      duplicate = strcmp( ((arrptr->hash)[idx])->license_no, (arrptr->arr)[line].license_no );
      #endif
      #ifdef HASH3
      duplicate = strcmp( ((arrptr->hash)[idx])->issue_date, (arrptr->arr)[line].issue_date );
      #endif
      
      if (duplicate == 0) {
        duplicate = 1;
      } else {
        duplicate = 0;
      }
      
      if (duplicate == 1) /*** if duplicate, skip */
      {
        printf( "  Skipping duplicate\n" );
        duplicates++;
        break;
      }
      printf( "  collision at %d %s\n", idx, ((arrptr->hash)[idx])->last_name );
      collisions++;
      idx++;
      if (idx>=arrptr->hash_size)
      {
        idx = 0;
      }
      
    }
    if (!duplicate)
    {
      printf("  inserting at %d\n", idx );
      (arrptr->hash)[idx] = (arrptr->arr)+line;
    }
  }

}

void free_array_ptr( struct array *ptr )
{
  int i;

  for (i=0;i<ptr->nelements;i++)
  {
    free( ptr->arr[i].last_name );
    free( ptr->arr[i].first_name );
    free( ptr->arr[i].license_type );
  }

  free( ptr->arr );
  free( ptr->hash );

  free( ptr );
}

struct record *find( char *key, struct array *arrptr )
{
  int idx;

  idx = str2int( key, arrptr->hash_size );
  while ( (arrptr->hash)[idx] != NULL ) /*** bucket is occupied */
  {
    if ( strcmp( key, ((arrptr->hash)[idx])->last_name )==0 )
    {
      return (arrptr->hash)[idx];
    }
    idx++;

    if (idx>=(arrptr->hash_size))
    {
      idx = 0;
    }
  }
  return NULL;

}


int main() /* DO NOT INCLUDE */
{
  struct array *arrptr;
  /*int line;*/
  /*struct record *r;*/

  collisions = 0;
  duplicates = 0;

  arrptr = read_records(); /*** building array */

  build_hash( arrptr, 500000 );

  /*for (line=0;line<arrptr->nelements;line++)
  {
    printf( " %d %s, %s: %s (%s). Issued: %s - %d\n", line,
	                    (arrptr->arr)[line].last_name,
	                    (arrptr->arr)[line].first_name,
                      (arrptr->arr)[line].license_type,
                      (arrptr->arr)[line].license_no,
                      (arrptr->arr)[line].issue_date,
                      str2int( (arrptr->arr)[line].last_name, 100 ) );
  }*/


  printf( "Duplicates: %d\n", duplicates );
  printf( "Collisions: %d\n", collisions );
  printf( "Total: %d\n", duplicates + collisions );

  /*
  r = find( "Kremalskdfjalsdkfjer", arrptr );
  if (r==NULL)
  {
    printf( "Not found\n" );
  }
  else
  {
    printf( "%s, %s : %s (%s). Issued: %s\n", r->last_name,
                             r->first_name,
                             r->license_type,
                             r->license_number,
                             r->issue_date);
  }
  */

  free_array_ptr( arrptr );
  return 0;
}
