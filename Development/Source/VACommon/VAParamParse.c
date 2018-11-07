#include <stdio.h>
#include "VAParamParse.h"

char* VAGetKVPairVal(const char *KVPairString, char KVSep, char KVPairSep, const char *key)
{
	const char *p1, *p2;
	const char *key_start, *key_end;
	const char *val_start, *val_end;
	char *val = NULL;
	int val_len = 0;
	int key_len = 0, key_len2 = 0;

	if (NULL == KVPairString || NULL == key)
	{
		return NULL;
	}

	key_len = strlen(key);

	/*
	 * 1. init: set search pos the start of KVPairString;
	 * 2. search key: 
	 *				if search pos == the end of KVPairString
	 *					goto 5
	 *				else
	 *					find start pos: search forward, the first ch not space(skip space);
	 *					find end pos: find the KVSep(pos=kvs_p), then search backward, the first ch not space(skip space);
	 *					find the KVPairSep(pos=kvps_p);
	 * 3. do key match(the first match):
	 *				if match: goto 4;
	 *				else: set search pos(kvps_p+1), goto 2;
	 * 4. search value:
	 *				find start pos: set search pos(kvs_p+1), search forward, the first ch not space(skip space);
	 *				find end pos: set search pos(kvps_p-1), then search backward, the first ch not space(skip space);
	 * 5. return value.
	 */
	p1 = p2 = KVPairString;
	while ('\0' != *p1)
	{
		/* search the key word -start- */
		while (' ' == *p1) /* skip space in key front */
			++p1;
		if ('\0' == *p1) /* if reach the end in the stage of key-search, then exit */
		    goto exit;
		/* p1 now point to the first non-space char */
		
		p2 = key_start = p1;	
		while (p2 && KVSep != *p2 && *p2 != '\0')
			++p2;
		if ('\0' == *p2) /* if reach the end in the stage of key-search, then exit*/
			goto exit;
		/* p2 now point to the KVSep*/	
		
		key_end = p2 - 1;
		while (key_end > key_start) /* strip space in key end */
		{
			if (' ' != *key_end)
			{
				break;
			}
			--key_end;
		}
		key_len2 = key_end - key_start + 1;
		/* search the key word -end- */
		
		
		p1 = p2 + 1;
		while ((KVPairSep != *p1) && ('\0' != *p1)) /* search the KVPairSep */
			++p1;
		/* p1 now point to the KVPairSep(or end) */
		
		
		/* search the value -start- */
		if (key_len2 == key_len && !strncmp(key_start, key, key_len)) /* do key match */
		{
			val_start = p2 + 1; /* right char of KVSep */
			val_end = p1 - 1;  /* left char of KVPairsep (or end) */
			
			while (' ' == *val_start) /* skip the space in value start*/
				++val_start;

			while (val_end > val_start)
			{
				if (' ' != *val_end)
				{
					break;
				}
				--val_end;
			}

			val_len = (int)(val_end - val_start + 1);
			if (val_len > 0)
			{
				val = (char *)malloc(val_len + 1);
				if (NULL != val)
				{
					memcpy((void *)val, (const void *)val_start, val_len);
					val[val_len] = '\0';
					break;
				}
			}
		}
		/* search the value -end- */

		if (KVPairSep == *p1) /* skip the separator*/
			++p1;
	}
	
exit:
	return val;
}