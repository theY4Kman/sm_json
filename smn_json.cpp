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

#include "smn_json.h"
#include "json_handle.h"
#include <jansson.h>

cell_t json_Parse(IPluginContext *pContext, const cell_t *params)
{
	char const *sz_json;

	pContext->LocalToString(params[1], (char **)&sz_json);
	
	json_error_t error;
	json_t *root = json_loads(sz_json, 0, &error);

	if (NULL == root)
	{
		char error_msg[JSON_ERROR_TEXT_LENGTH + 32];
		smutils->Format((char *)&error_msg, sizeof(error_msg), "Line %d:%d: %s",
			error.line, error.column, error.text);

		pContext->StringToLocalUTF8(params[2], params[3], (char *)&error_msg, NULL);
		return BAD_HANDLE;
	}

	return handlesys->CreateHandle(g_JSONType,
		root,
		pContext->GetIdentity(),
		myself->GetIdentity(),
		NULL);
}

cell_t json_Dumps(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
 
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	json_t *value = NULL;
	if ((err = handlesys->ReadHandle(hndl, g_JSONType, &sec, (void **)&value))
	     != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid JSON value handle %x (error %d)", hndl, err);
	}

	char *encoded_value = json_dumps(value, params[4]);

	size_t numBytes = 0;
	pContext->StringToLocalUTF8(params[2], params[3], encoded_value, &numBytes);
	free((void *)encoded_value);

	return numBytes;
}

cell_t json_Type(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
 
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	json_t *value = NULL;
	if ((err = handlesys->ReadHandle(hndl, g_JSONType, &sec, (void **)&value))
	     != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid JSON value handle %x (error %d)", hndl, err);
	}

	return json_typeof(value);
}

cell_t json_ReadString(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
 
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	json_t *value = NULL;
	if ((err = handlesys->ReadHandle(hndl, g_JSONType, &sec, (void **)&value))
	     != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid JSON value handle %x (error %d)", hndl, err);
	}

	if (!json_is_string(value))
		return -1;

	size_t numBytes = 0;
	pContext->StringToLocalUTF8(params[2], params[3], json_string_value(value), &numBytes);

	return numBytes;
}

cell_t json_ReadCell(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
 
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	json_t *value = NULL;
	if ((err = handlesys->ReadHandle(hndl, g_JSONType, &sec, (void **)&value))
	     != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid JSON value handle %x (error %d)", hndl, err);
	}

	if (!json_is_integer(value))
		return -1;

	return (cell_t)json_integer_value(value);
}

cell_t json_ReadFloat(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
 
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	json_t *value = NULL;
	if ((err = handlesys->ReadHandle(hndl, g_JSONType, &sec, (void **)&value))
	     != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid JSON value handle %x (error %d)", hndl, err);
	}

	if (!json_is_real(value))
		return -1;

	return sp_ftoc((float)json_real_value(value));
}

cell_t json_ArraySize(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
 
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	json_t *value = NULL;
	if ((err = handlesys->ReadHandle(hndl, g_JSONType, &sec, (void **)&value))
	     != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid JSON value handle %x (error %d)", hndl, err);
	}

	if (!json_is_array(value))
		return -1;

	return json_array_size(value);
}

cell_t json_ArrayGet(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
 
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	json_t *value = NULL;
	if ((err = handlesys->ReadHandle(hndl, g_JSONType, &sec, (void **)&value))
	     != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid JSON value handle %x (error %d)", hndl, err);
	}

	if (!json_is_array(value))
		return BAD_HANDLE;

	cell_t index = params[2];
	if (index < 0 || index >= (cell_t)json_array_size(value))
		return BAD_HANDLE;

	json_t *object = json_array_get(value, index);
	if (NULL == object)
		return BAD_HANDLE;

	return handlesys->CreateHandle(g_JSONType,
		object,
		pContext->GetIdentity(),
		myself->GetIdentity(),
		NULL);
}

cell_t json_ObjectSize(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
 
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	json_t *value = NULL;
	if ((err = handlesys->ReadHandle(hndl, g_JSONType, &sec, (void **)&value))
	     != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid JSON value handle %x (error %d)", hndl, err);
	}

	if (!json_is_object(value))
		return -1;

	return json_object_size(value);
}

cell_t json_ObjectGet(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
 
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	json_t *value = NULL;
	if ((err = handlesys->ReadHandle(hndl, g_JSONType, &sec, (void **)&value))
	     != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid JSON value handle %x (error %d)", hndl, err);
	}
	
	if (!json_is_object(value))
		return BAD_HANDLE;

	char const *key;
	pContext->LocalToString(params[2], (char **)&key);

	json_t *object = json_object_get(value, key);
	if (NULL == object)
		return BAD_HANDLE;

	return handlesys->CreateHandle(g_JSONType,
		object,
		pContext->GetIdentity(),
		myself->GetIdentity(),
		NULL);
}

cell_t json_ObjectIter(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
 
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	json_t *value = NULL;
	if ((err = handlesys->ReadHandle(hndl, g_JSONType, &sec, (void **)&value))
	     != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid JSON value handle %x (error %d)", hndl, err);
	}
	
	if (!json_is_object(value))
		return BAD_HANDLE;

	void *iter = json_object_iter(value);
	if (NULL == iter)
		return BAD_HANDLE;

	sm_json_iter_t *iter_wrapper = new sm_json_iter_t(value, iter);

	return handlesys->CreateHandle(g_JSONIterType,
		iter_wrapper,
		pContext->GetIdentity(),
		myself->GetIdentity(),
		NULL);
}

cell_t json_ObjectIterRead(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
 
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	sm_json_iter_t *iter_wrapper = NULL;
	if ((err = handlesys->ReadHandle(hndl, g_JSONIterType, &sec, (void **)&iter_wrapper))
	     != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid JSON object iterator handle %x (error %d)", hndl, err);
	}

	if (NULL == iter_wrapper->iter || NULL == iter_wrapper->object)
	{
		handlesys->FreeHandle(hndl, &sec);
		return BAD_HANDLE;
	}

	char const *key = json_object_iter_key(iter_wrapper->iter);
	pContext->StringToLocalUTF8(params[2], params[3], key, NULL);

	json_t *value = json_object_iter_value(iter_wrapper->iter);
	json_incref(value);

	return handlesys->CreateHandle(g_JSONType,
		value,
		pContext->GetIdentity(),
		myself->GetIdentity(),
		NULL);
}

cell_t json_ObjectIterNext(IPluginContext *pContext, const cell_t *params)
{
	Handle_t hndl = static_cast<Handle_t>(params[1]);
	HandleError err;
	HandleSecurity sec;
 
	sec.pOwner = NULL;
	sec.pIdentity = myself->GetIdentity();

	sm_json_iter_t *iter_wrapper = NULL;
	if ((err = handlesys->ReadHandle(hndl, g_JSONIterType, &sec, (void **)&iter_wrapper))
	     != HandleError_None)
	{
		return pContext->ThrowNativeError("Invalid JSON object iterator handle %x (error %d)", hndl, err);
	}

	if (NULL == iter_wrapper->iter || NULL == iter_wrapper->object)
	{
		handlesys->FreeHandle(hndl, &sec);
		return BAD_HANDLE;
	}

	iter_wrapper->iter = json_object_iter_next(iter_wrapper->object, iter_wrapper->iter);

	if (NULL == iter_wrapper->iter || NULL == iter_wrapper->object)
	{
		handlesys->FreeHandle(hndl, &sec);
		return BAD_HANDLE;
	}

	return static_cast<cell_t>(hndl);
}

sp_nativeinfo_t JSON_Natives[] = {
	{"JSON_Parse",			json_Parse},
	{"JSON_Dumps",			json_Dumps},
	{"JSON_Type",			json_Type},
	{"JSON_ReadString",		json_ReadString},
	{"JSON_ReadCell",		json_ReadCell},
	{"JSON_ReadFloat",		json_ReadFloat},
	{"JSON_ArraySize",		json_ArraySize},
	{"JSON_ArrayGet",		json_ArrayGet},
	{"JSON_ObjectSize",		json_ObjectSize},
	{"JSON_ObjectGet",		json_ObjectGet},
	{"JSON_ObjectIter",		json_ObjectIter},
	{"JSON_ObjectIterRead",	json_ObjectIterRead},
	{"JSON_ObjectIterNext",	json_ObjectIterNext},
	{NULL,					NULL}
};
