/* oo_COPY.h
 *
 * Copyright (C) 1994-2011 Paul Boersma
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "oo_undef.h"

#define oo_SIMPLE(type,storage,x)  \
	thy x = my x;

#define oo_ARRAY(type,storage,x,cap,n)  \
	for (int i = 0; i < n; i ++) \
		thy x [i] = my x [i];

#define oo_SET(type,storage,x,setType)  \
	for (int i = 0; i <= setType##_MAX; i ++) \
		thy x [i] = my x [i];

#define oo_VECTOR(type,t,storage,x,min,max)  \
	if (my x) thy x = NUMvector_copy <type> (my x, min, max);

#define oo_MATRIX(type,t,storage,x,row1,row2,col1,col2)  \
	if (my x) thy x = NUMmatrix_copy <type> (my x, row1, row2, col1, col2);

#define oo_ENUMx(type,storage,Type,x)  \
	thy x = my x;

#define oo_ENUMx_ARRAY(type,storage,Type,x,cap,n)  \
	for (int i = 0; i < n; i ++) thy x [i] = my x [i];

#define oo_ENUMx_SET(type,storage,Type,x,setType)  \
	for (int i = 0; i <= setType##_MAX; i ++) thy x [i] = my x [i];

#define oo_ENUMx_VECTOR(type,t,storage,Type,x,min,max)  \
	if (my x) thy x = NUM##t##vector_copy (my x, min, max);

#define oo_STRINGx(storage,x)  \
	if (my x) thy x = Melder_wcsdup (my x);

#define oo_STRINGx_ARRAY(storage,x,cap,n)  \
	for (int i = 0; i < n; i ++) \
		if (my x [i]) thy x [i] = Melder_wcsdup (my x [i]);

#define oo_STRINGx_SET(storage,x,setType)  \
	for (int i = 0; i <= setType##_MAX; i ++) \
		if (my x [i]) thy x [i] = Melder_wcsdup (my x [i]);

#define oo_STRINGx_VECTOR(storage,x,min,max)  \
	if (my x) { \
		thy x = NUMvector <wchar*> (min, max); \
		for (long i = min; i <= max; i ++) \
			if (my x [i]) thy x [i] = Melder_wcsdup (my x [i]); \
	}

#define oo_STRUCT(Type,x)  \
	Type##_copy (& my x, & thy x);

#define oo_STRUCT_ARRAY(Type,x,cap,n)  \
	for (int i = 0; i < n; i ++) \
		Type##_copy (& my x [i], & thy x [i]);

#define oo_STRUCT_SET(Type,x,setType)  \
	for (int i = 0; i <= setType##_MAX; i ++) \
		Type##_copy (& my x [i], & thy x [i]);

#define oo_STRUCT_VECTOR_FROM(Type,x,min,max)  \
	if (my x) { \
		thy x = NUMvector <struct##Type> (min, max); \
		for (long i = min; i <= max; i ++) \
			Type##_copy (& my x [i], & thy x [i]); \
	}

#define oo_STRUCT_MATRIX_FROM(Type,x,row1,row2,col1,col2)  \
	if (my x) { \
		thy x = NUMmatrix <struct##Type> (row1, row2, col1, col2); \
		for (long i = row1; i <= row2; i ++) \
			for (long j = col1; j <= col2; j ++) \
				Type##_copy (& my x [i] [j], & thy x [i] [j]); \
	}

#define oo_WIDGET(x)  \
	!!!! Can never copy Widgets... !!!!

#define oo_WIDGET_ARRAY(x,cap,n)  \
	!!!! Can never copy Widgets... !!!!

#define oo_WIDGET_SET(x,setType)  \
	!!!! Can never copy Widgets... !!!!

#define oo_WIDGET_VECTOR_FROM(x,cap,min,max)  \
	!!!! Can never copy Widgets... !!!!

#define oo_OBJECT(Class,version,x)  \
	if (my x) thy x = (Class) Data_copy (my x);

#define oo_COLLECTION(Class,x,ItemClass,version)  \
	if (my x) thy x = (Class) Data_copy (my x);

#define oo_FILE(x)  \
	MelderFile_copy (& my x, & thy x);

#define oo_DIR(x)  \
	MelderDir_copy (& my x, & thy x);

#define oo_DEFINE_STRUCT(Type)  \
	void Type##_copy (Type me, Type thee) {

#define oo_END_STRUCT(Type)  \
	}

#define oo_DEFINE_CLASS(Class,Parent)  \
	static void class##Class##_copy (I, thou) { \
		iam (Class); \
		thouart (Class); \
		inherited (Class) copy (me, thee);

#define oo_END_CLASS(Class)  \
	}

#define oo_IF(condition)  \
	if (condition) {

#define oo_ENDIF  \
	}

#define oo_FROM(from)

#define oo_ENDFROM

#define oo_VERSION(version)

#define oo_DECLARING  0
#define oo_DESTROYING  0
#define oo_COPYING  1
#define oo_EQUALLING  0
#define oo_COMPARING  0
#define oo_VALIDATING_ENCODING  0
#define oo_READING  0
#define oo_READING_TEXT  0
#define oo_READING_BINARY  0
#define oo_READING_CACHE  0
#define oo_READING_LISP  0
#define oo_WRITING  0
#define oo_WRITING_TEXT  0
#define oo_WRITING_BINARY  0
#define oo_WRITING_CACHE  0
#define oo_WRITING_LISP  0
#define oo_DESCRIBING  0

/* End of file oo_COPY.h */
