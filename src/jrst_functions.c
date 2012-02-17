/* Do not edit. File generated by rastro. */

#include "jrst_functions.h"

/* Rastro function implementation for 'i' */
void rst_event_i_ptr(rst_buffer_t *ptr, u_int16_t type, u_int32_t i0)
{
  rst_startevent(ptr, type<<18|0x27000);
  RST_PUT(ptr, u_int32_t, i0);
  rst_endevent(ptr);
}
/* Rastro function implementation for 'i' - fortran support */
void rst_event_i_f_( int16_t *type, int32_t* i0)
{
  rst_event_i ((u_int16_t)* type, (u_int32_t)* i0);
}

/* Rastro function implementation for 'ii' */
void rst_event_ii_ptr(rst_buffer_t *ptr, u_int16_t type, u_int32_t i0, u_int32_t i1)
{
  rst_startevent(ptr, type<<18|0x27700);
  RST_PUT(ptr, u_int32_t, i0);
  RST_PUT(ptr, u_int32_t, i1);
  rst_endevent(ptr);
}
/* Rastro function implementation for 'ii' - fortran support */
void rst_event_ii_f_( int16_t *type, int32_t* i0, int32_t* i1)
{
  rst_event_ii ((u_int16_t)* type, (u_int32_t)* i0, (u_int32_t)* i1);
}

/* Rastro function implementation for 'is' */
void rst_event_is_ptr(rst_buffer_t *ptr, u_int16_t type, u_int32_t i0, const char* s0)
{
  rst_startevent(ptr, type<<18|0x27100);
  RST_PUT(ptr, u_int32_t, i0);
  RST_PUT_STR(ptr, s0);
  rst_endevent(ptr);
}
/* Rastro function implementation for 'is' - fortran support */
void rst_event_is_f_( int16_t *type, int32_t* i0, const char* s0)
{
  rst_event_is ((u_int16_t)* type, (u_int32_t)* i0,  s0);
}

/* Rastro function implementation for 'isiii' */
void rst_event_isiii_ptr(rst_buffer_t *ptr, u_int16_t type, u_int32_t i0, const char* s0, u_int32_t i1, u_int32_t i2, u_int32_t i3)
{
  rst_startevent(ptr, type<<18|0x7777);
  RST_PUT(ptr, u_int32_t, 0x21000000);
  RST_PUT(ptr, u_int32_t, i0);
  RST_PUT(ptr, u_int32_t, i1);
  RST_PUT(ptr, u_int32_t, i2);
  RST_PUT(ptr, u_int32_t, i3);
  RST_PUT_STR(ptr, s0);
  rst_endevent(ptr);
}
/* Rastro function implementation for 'isiii' - fortran support */
void rst_event_isiii_f_( int16_t *type, int32_t* i0, const char* s0, int32_t* i1, int32_t* i2, int32_t* i3)
{
  rst_event_isiii ((u_int16_t)* type, (u_int32_t)* i0,  s0, (u_int32_t)* i1, (u_int32_t)* i2, (u_int32_t)* i3);
}

/* Rastro function implementation for 'liis' */
void rst_event_liis_ptr(rst_buffer_t *ptr, u_int16_t type, u_int64_t l0, u_int32_t i0, u_int32_t i1, const char* s0)
{
  rst_startevent(ptr, type<<18|0x24771);
  RST_PUT(ptr, u_int64_t, l0);
  RST_PUT(ptr, u_int32_t, i0);
  RST_PUT(ptr, u_int32_t, i1);
  RST_PUT_STR(ptr, s0);
  rst_endevent(ptr);
}
/* Rastro function implementation for 'liis' - fortran support */
void rst_event_liis_f_( int16_t *type, int64_t* l0, int32_t* i0, int32_t* i1, const char* s0)
{
  rst_event_liis ((u_int16_t)* type, (u_int64_t)* l0, (u_int32_t)* i0, (u_int32_t)* i1,  s0);
}

/* Rastro function implementation for 'lis' */
void rst_event_lis_ptr(rst_buffer_t *ptr, u_int16_t type, u_int64_t l0, u_int32_t i0, const char* s0)
{
  rst_startevent(ptr, type<<18|0x24710);
  RST_PUT(ptr, u_int64_t, l0);
  RST_PUT(ptr, u_int32_t, i0);
  RST_PUT_STR(ptr, s0);
  rst_endevent(ptr);
}
/* Rastro function implementation for 'lis' - fortran support */
void rst_event_lis_f_( int16_t *type, int64_t* l0, int32_t* i0, const char* s0)
{
  rst_event_lis ((u_int16_t)* type, (u_int64_t)* l0, (u_int32_t)* i0,  s0);
}

void rst_init_f_(int64_t *id1, int64_t *id2)
{
  rst_init((u_int64_t)* id1, (u_int64_t)* id2);
}
void rst_finalize_f_ ()
{
  rst_finalize();
}