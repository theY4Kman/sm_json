/**
 * vim: set ts=4 :
 * =============================================================================
 * SM JSON
 * Copyright (C) 2012 Zach "theY4Kman" Kanzler
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "sdk\smsdk_ext.h"
#include <jansson.h>

extern HandleType_t g_JSONType;
extern HandleType_t g_JSONIterType;


typedef struct sm_json_iter {
	sm_json_iter(json_t *object, void *iter) : object(object), iter(iter)
		{  };

	json_t *object;
	void *iter;
} sm_json_iter_t;


class JSONTypeHandler : public IHandleTypeDispatch
{
public:
	void OnHandleDestroy(HandleType_t type, void *object);
};

class JSONIterTypeHandler : public IHandleTypeDispatch
{
public:
	void OnHandleDestroy(HandleType_t type, void *object);
};


extern JSONTypeHandler g_JSONTypeHandler;
extern JSONIterTypeHandler g_JSONIterTypeHandler;
