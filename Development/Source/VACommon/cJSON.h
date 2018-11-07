/*
  Copyright (c) 2009 Dave Gamble
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef cJSON__h
#define cJSON__h

#include "mem_pool.h"

/* cJSON Types: */
#define cJSON_False 0
#define cJSON_True 1
#define cJSON_NULL 2
#define cJSON_Number 3
#define cJSON_String 4
#define cJSON_Array 5
#define cJSON_Object 6
	
#define cJSON_IsReference 256

/* The cJSON structure: */
typedef struct cJSON {
	struct cJSON *next,*prev;	/* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
	struct cJSON *child;		/* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */

	int type;					/* The type of the item, as above. */

	char *valuestring;			/* The item's string, if type==cJSON_String */
	int valueint;				/* The item's number, if type==cJSON_Number */
	double valuedouble;			/* The item's number, if type==cJSON_Number */

	char *string;				/* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
} cJSON;

typedef void* (*PCBJsonMalloc)(size_t sz);
typedef void (*PCBJsonFree)(void *ptr);

typedef struct cJSON_Hooks {
      PCBJsonMalloc malloc_fn;
      PCBJsonFree free_fn;
} cJSON_Hooks;

namespace VA {

class JsonObj{
private:
    const char *ep;

    ivhMemPool m_hMemPool;
    void* cJSON_malloc(size_t sz);

    int cJSON_strcasecmp(const char *s1,const char *s2);
    char* cJSON_strdup(const char* str);
    cJSON *cJSON_New_Item(void);
    const char *parse_number(cJSON *item,const char *num);
    char *print_number(cJSON *item);
    unsigned parse_hex4(const char *str);
    const char *parse_string(cJSON *item,const char *str);
    char *print_string_ptr(const char *str);
    char *print_string(cJSON *item);
    const char *parse_value(cJSON *item,const char *value);
    char *print_value(cJSON *item,int depth,int fmt);
    const char *parse_array(cJSON *item,const char *value);
    char *print_array(cJSON *item,int depth,int fmt);
    const char *parse_object(cJSON *item,const char *value);
    char *print_object(cJSON *item,int depth,int fmt);
    const char *skip(const char *in);
    void suffix_object(cJSON *prev,cJSON *item);
    cJSON *create_reference(cJSON *item);

public:
    JsonObj();
    virtual ~JsonObj();

    void  cJSON_free(void *ptr);

/* Supply a block of JSON, and this returns a cJSON object you can interrogate. Call cJSON_Delete when finished. */
    cJSON *cJSON_Parse(const char *value);
/* Render a cJSON entity to text for transfer/storage. Free the char* when finished. */
    char  *cJSON_Print(cJSON *item);
/* Render a cJSON entity to text for transfer/storage without any formatting. Free the char* when finished. */
    char  *cJSON_PrintUnformatted(cJSON *item);
/* Delete a cJSON entity and all subentities. */
    void   cJSON_Delete(cJSON *c);

/* Returns the number of items in an array (or object). */
    int	  cJSON_GetArraySize(cJSON *array);
/* Retrieve item number "item" from array "array". Returns NULL if unsuccessful. */
    cJSON *cJSON_GetArrayItem(cJSON *array,int item);
/* Get item "string" from object. Case insensitive. */
    cJSON *cJSON_GetObjectItem(cJSON *object,const char *string);

/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when cJSON_Parse() returns 0. 0 when cJSON_Parse() succeeds. */
    const char *cJSON_GetErrorPtr(void);
	
/* These calls create a cJSON item of the appropriate type. */
    cJSON *cJSON_CreateNull(void);
    cJSON *cJSON_CreateTrue(void);
    cJSON *cJSON_CreateFalse(void);
    cJSON *cJSON_CreateBool(int b);
    cJSON *cJSON_CreateNumber(double num);
    cJSON *cJSON_CreateString(const char *string);
    cJSON *cJSON_CreateArray(void);
    cJSON *cJSON_CreateObject(void);

/* These utilities create an Array of count items. */
    cJSON *cJSON_CreateIntArray(const int *numbers,int count);
    cJSON *cJSON_CreateFloatArray(const float *numbers,int count);
    cJSON *cJSON_CreateDoubleArray(const double *numbers,int count);
    cJSON *cJSON_CreateStringArray(const char **strings,int count);

/* Append item to the specified array/object. */
    void cJSON_AddItemToArray(cJSON *array, cJSON *item);
    void	cJSON_AddItemToObject(cJSON *object,const char *string,cJSON *item);
/* Append reference to item to the specified array/object. Use this when you want to add an existing cJSON to a new cJSON, but don't want to corrupt your existing cJSON. */
    void cJSON_AddItemReferenceToArray(cJSON *array, cJSON *item);
    void	cJSON_AddItemReferenceToObject(cJSON *object,const char *string,cJSON *item);

/* Remove/Detatch items from Arrays/Objects. */
    cJSON *cJSON_DetachItemFromArray(cJSON *array,int which);
    void   cJSON_DeleteItemFromArray(cJSON *array,int which);
    cJSON *cJSON_DetachItemFromObject(cJSON *object,const char *string);
    void   cJSON_DeleteItemFromObject(cJSON *object,const char *string);
	
/* Update array items. */
    void cJSON_ReplaceItemInArray(cJSON *array,int which,cJSON *newitem);
    void cJSON_ReplaceItemInObject(cJSON *object,const char *string,cJSON *newitem);

/* Duplicate a cJSON item */
    cJSON *cJSON_Duplicate(cJSON *item,int recurse);
/* Duplicate will create a new, identical cJSON item to the one you pass, in new memory that will
need to be released. With recurse!=0, it will duplicate any children connected to the item.
The item->next and ->prev pointers are always zero on return from Duplicate. */

/* ParseWithOpts allows you to require (and check) that the JSON is null terminated, and to retrieve the pointer to the final byte parsed. */
    cJSON *cJSON_ParseWithOpts(const char *value,const char **return_parse_end,int require_null_terminated);

    void cJSON_Minify(char *json);

/* When assigning an integer value, it needs to be propagated to valuedouble too. */
    void cJSON_SetIntValue(cJSON *object,int val) {
        if(object) {
            (object)->valuedouble=(val);
			(object)->valueint=static_cast<int>((object)->valuedouble);
        }
    }
    void cJSON_SetIntValue(cJSON *object,double val) {
        if(object) {
            (object)->valuedouble=(val);
			(object)->valueint=static_cast<int>((object)->valuedouble);
        }
    }
};

}   // namespace VA

#endif
