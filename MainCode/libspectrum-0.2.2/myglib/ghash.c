/* ghash.c: Minimal replacement for GHashTable

   This code taken almost verbatim from glib 2.4.0's glib/ghash.c
   Copyright (C) 1995-1998  Peter Mattis, Spencer Kimball and Josh MacDonald

   Modified by the GLib Team and others 1997-2000.  See the AUTHORS
   file for a list of people on the GLib Team.  See the ChangeLog
   files for a list of changes.  These files are distributed with GLib
   at ftp://ftp.gtk.org/pub/gtk/.

   Modified by Philip Kendall 2004.

   $Id: ghash.c,v 1.3 2004/04/16 14:15:20 fredm Exp $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

   Author contact information:

   Philip Kendall <pak21-fuse@srcf.ucam.org>
   Postal address: 15 Crescent Road, Wokingham, Berks, RG40 2DB, England

*/

#include <config.h>

#ifndef HAVE_LIB_GLIB		/* Use this iff we're not using the 
				   `proper' glib */

#include "internals.h"

#define HASH_TABLE_SIZE 241

typedef struct _GHashNode      GHashNode;

struct _GHashNode
{
  gpointer   key;
  gpointer   value;
  GHashNode *next;
};

struct _GHashTable
{
  GHashNode   **nodes;
  GHashFunc	hash_func;
  GCompareFunc	key_equal_func;
};

static GHashNode *node_free_list = NULL;

GHashTable*
g_hash_table_new (GHashFunc	hash_func,
		  GCompareFunc	key_equal_func)
{
  GHashTable *hash_table;
  guint i;

  hash_table = malloc (sizeof (GHashTable));
  if (!hash_table)
    return NULL;

  hash_table->hash_func = hash_func;
  hash_table->key_equal_func = key_equal_func;
  hash_table->nodes = malloc (HASH_TABLE_SIZE * sizeof (GHashNode*));
  if (!hash_table->nodes)
    {
      free (hash_table);
      return NULL;
    }

  for (i = 0; i < HASH_TABLE_SIZE; i++)
    hash_table->nodes[i] = NULL;

  return hash_table;
}

static void
g_hash_nodes_destroy (GHashNode *hash_node)
{
  if (hash_node)
    {
      GHashNode *node = hash_node;

      while (node->next)
	node = node->next;

      node->next = node_free_list;
      node_free_list = hash_node;
    }
}

void
g_hash_table_destroy (GHashTable *hash_table)
{
  guint i;
  
  for (i = 0; i < HASH_TABLE_SIZE; i++)
    g_hash_nodes_destroy (hash_table->nodes[i]);
  
  free (hash_table->nodes);
  free (hash_table);
}

static GHashNode**
g_hash_table_lookup_node (GHashTable    *hash_table,
                          gconstpointer  key)
{
  GHashNode **node;
  
  node = &hash_table->nodes
    [(* hash_table->hash_func) (key) % HASH_TABLE_SIZE];
  
  while (*node && !(*hash_table->key_equal_func) ((*node)->key, key))
    node = &(*node)->next;
  
  return node;
}

gpointer
g_hash_table_lookup (GHashTable   *hash_table,
		     gconstpointer key)
{
  GHashNode *node;

  node = *g_hash_table_lookup_node (hash_table, key);

  return node ? node->value : NULL;
}

static GHashNode*
g_hash_node_new (gpointer key,
                 gpointer value)
{
  GHashNode *hash_node;
  guint i;

  if (!node_free_list)
    {
      node_free_list = malloc (1024 * sizeof (GHashNode));
      if (!node_free_list)
	return NULL;

      for(i = 0; i < 1023; i++ )
	node_free_list[i].next = &node_free_list[i+1];
      node_free_list[1023].next = NULL;
    }

  
  hash_node = node_free_list;
  node_free_list = node_free_list->next;
  
  hash_node->key = key;
  hash_node->value = value;
  hash_node->next = NULL;
  
  return hash_node;
}

void
g_hash_table_insert (GHashTable *hash_table,
                     gpointer    key,
                     gpointer    value)
{
  GHashNode **node;
  
  node = g_hash_table_lookup_node (hash_table, key);
  
  if (*node)
    {
      (*node)->value = value;
    }
  else
    {
      *node = g_hash_node_new (key, value);
    }
}

static void
g_hash_node_destroy (GHashNode *hash_node)
{
  hash_node->next = node_free_list;
  node_free_list = hash_node;
}

guint
g_hash_table_foreach_remove (GHashTable *hash_table,
                             GHRFunc     func,
                             gpointer    user_data)
{
  GHashNode *node, *prev;
  guint i;
  guint deleted = 0;
  
  for (i = 0; i < HASH_TABLE_SIZE; i++)
    {
    restart:
      
      prev = NULL;
      
      for (node = hash_table->nodes[i]; node; prev = node, node = node->next)
        {
          if ((* func) (node->key, node->value, user_data))
            {
              deleted += 1;
              
              if (prev)
                {
                  prev->next = node->next;
                  g_hash_node_destroy (node);
                  node = prev;
                }
              else
                {
                  hash_table->nodes[i] = node->next;
                  g_hash_node_destroy (node);
                  goto restart;
                }
            }
        }
    }
  
  return deleted;
}

guint
g_int_hash (gconstpointer v)
{
  return *(const gint*) v;
}

gboolean
g_int_equal (gconstpointer v1,
             gconstpointer v2)
{
  return *((const gint*) v1) == *((const gint*) v2);
}

#endif				/* #ifndef HAVE_LIB_GLIB */
