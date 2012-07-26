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

#include "json_handle.h"

HandleType_t g_JSONType = 0;
HandleType_t g_JSONIterType = 0;


void JSONTypeHandler::OnHandleDestroy(HandleType_t type, void *object)
{
	json_decref((json_t *)object);
}

void JSONIterTypeHandler::OnHandleDestroy(HandleType_t type, void *object)
{
	json_t *value = ((sm_json_iter_t *)object)->object;
	if (NULL != value)
		json_decref(value);

	free(object);
}

JSONTypeHandler g_JSONTypeHandler;
JSONIterTypeHandler g_JSONIterTypeHandler;
