# Constants declarations.
CC=gcc
CPPC=g++
CPPCFLAGS=-O2 -I../include/ -DINVALID -Wall -c
CPPCFLAGS=-O2 -I../include/ -DINVALID -Wall -c


# Objects
objects=\
	adler32.o \
	bignum.o \
	compress.o \
	crc32.o \
	deflate.o \
	gaia_db_dblocal.o \
	gaia_db_dbremote.o \
	gaia_digit_diff.o \
	gaia_digit_regexp.o \
	gaia_digit_zip.o \
	gaia_doc_pdf.o \
	gaia_img_base.o \
	gaia_img_jpeg.o \
	gaia_img_png.o \
	gaia_network_asyncdispatcher.o \
	gaia_network_asyncsocket.o \
	gaia_network_client.o \
	gaia_network_http.o \
	gaia_network_httpbase.o \
	gaia_network_httpserver.o \
	gaia_network_server.o \
	gaia_network_sudpsocket.o \
	gaia_stream_stdstream.o \
	gaia_type.o \
	gzclose.o \
	gzlib.o \
	gzread.o \
	gzwrite.o \
	hpdf_3dmeasure.o \
	hpdf_annotation.o \
	hpdf_array.o \
	hpdf_binary.o \
	hpdf_boolean.o \
	hpdf_catalog.o \
	hpdf_destination.o \
	hpdf_dict.o \
	hpdf_doc_png.o \
	hpdf_doc.o \
	hpdf_encoder_cns.o \
	hpdf_encoder_cnt.o \
	hpdf_encoder_jp.o \
	hpdf_encoder_kr.o \
	hpdf_encoder_utf.o \
	hpdf_encoder.o \
	hpdf_encrypt.o \
	hpdf_encryptdict.o \
	hpdf_error.o \
	hpdf_exdata.o \
	hpdf_ext_gstate.o \
	hpdf_font_cid.o \
	hpdf_font_tt.o \
	hpdf_font_type1.o \
	hpdf_font.o \
	hpdf_fontdef_base14.o \
	hpdf_fontdef_cid.o \
	hpdf_fontdef_cns.o \
	hpdf_fontdef_cnt.o \
	hpdf_fontdef_jp.o \
	hpdf_fontdef_kr.o \
	hpdf_fontdef_tt.o \
	hpdf_fontdef_type1.o \
	hpdf_fontdef.o \
	hpdf_gstate.o \
	hpdf_image_ccitt.o \
	hpdf_image_png.o \
	hpdf_image.o \
	hpdf_info.o \
	hpdf_list.o \
	hpdf_mmgr.o \
	hpdf_name.o \
	hpdf_namedict.o \
	hpdf_null.o \
	hpdf_number.o \
	hpdf_objects.o \
	hpdf_outline.o \
	hpdf_page_label.o \
	hpdf_page_operator.o \
	hpdf_pages.o \
	hpdf_pdfa.o \
	hpdf_real.o \
	hpdf_streams.o \
	hpdf_string.o \
	hpdf_u3d.o \
	hpdf_utils.o \
	hpdf_xref.o \
	infback.o \
	inffast.o \
	inflate.o \
	inftrees.o \
	pcre_byte_order.o \
	pcre_chartables.o \
	pcre_compile.o \
	pcre_config.o \
	pcre_dfa_exec.o \
	pcre_exec.o \
	pcre_fullinfo.o \
	pcre_get.o \
	pcre_globals.o \
	pcre_jit_compile.o \
	pcre_maketables.o \
	pcre_newline.o \
	pcre_ord2utf8.o \
	pcre_printint.o \
	pcre_refcount.o \
	pcre_string_utils.o \
	pcre_study.o \
	pcre_tables.o \
	pcre_ucd.o \
	pcre_valid_utf8.o \
	pcre_version.o \
	pcre_xclass.o \
	pcre16_byte_order.o \
	pcre16_chartables.o \
	pcre16_compile.o \
	pcre16_config.o \
	pcre16_dfa_exec.o \
	pcre16_exec.o \
	pcre16_fullinfo.o \
	pcre16_get.o \
	pcre16_globals.o \
	pcre16_jit_compile.o \
	pcre16_maketables.o \
	pcre16_newline.o \
	pcre16_ord2utf16.o \
	pcre16_printint.o \
	pcre16_refcount.o \
	pcre16_string_utils.o \
	pcre16_study.o \
	pcre16_tables.o \
	pcre16_ucd.o \
	pcre16_utf16_utils.o \
	pcre16_valid_utf16.o \
	pcre16_version.o \
	pcre16_xclass.o \
	pcre32_byte_order.o \
	pcre32_chartables.o \
	pcre32_compile.o \
	pcre32_config.o \
	pcre32_dfa_exec.o \
	pcre32_exec.o \
	pcre32_fullinfo.o \
	pcre32_get.o \
	pcre32_globals.o \
	pcre32_jit_compile.o \
	pcre32_maketables.o \
	pcre32_newline.o \
	pcre32_ord2utf32.o \
	pcre32_printint.o \
	pcre32_refcount.o \
	pcre32_string_utils.o \
	pcre32_study.o \
	pcre32_tables.o \
	pcre32_ucd.o \
	pcre32_utf32_utils.o \
	pcre32_valid_utf32.o \
	pcre32_version.o \
	pcre32_xclass.o \
	png.o \
	pngerror.o \
	pngget.o \
	pngmem.o \
	pngpread.o \
	pngread.o \
	pngrio.o \
	pngrtran.o \
	pngrutil.o \
	pngset.o \
	pngtrans.o \
	pngwio.o \
	pngwrite.o \
	pngwtran.o \
	pngwutil.o \
	rsa.o \
	sqlite3.o \
	trees.o \
	uncompr.o \
	zutil.o 

# Link
link: $(objects)
	$(CPPC) $(objects) -o gaia


# Every objects
adler32.o: ../src/adler32.c
	$(CC) $(CPPCFLAGS) ../src/adler32.c

bignum.o: ../src/bignum.c
	$(CC) $(CPPCFLAGS) ../src/bignum.c

compress.o: ../src/compress.c
	$(CC) $(CPPCFLAGS) ../src/compress.c

crc32.o: ../src/crc32.c
	$(CC) $(CPPCFLAGS) ../src/crc32.c

deflate.o: ../src/deflate.c
	$(CC) $(CPPCFLAGS) ../src/deflate.c

gaia_db_dblocal.o: ../src/gaia_db_dblocal.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_db_dblocal.cpp

gaia_db_dbremote.o: ../src/gaia_db_dbremote.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_db_dbremote.cpp

gaia_digit_diff.o: ../src/gaia_digit_diff.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_digit_diff.cpp

gaia_digit_regexp.o: ../src/gaia_digit_regexp.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_digit_regexp.cpp

gaia_digit_zip.o: ../src/gaia_digit_zip.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_digit_zip.cpp

gaia_doc_pdf.o: ../src/gaia_doc_pdf.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_doc_pdf.cpp

gaia_img_base.o: ../src/gaia_img_base.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_img_base.cpp

gaia_img_jpeg.o: ../src/gaia_img_jpeg.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_img_jpeg.cpp

gaia_img_png.o: ../src/gaia_img_png.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_img_png.cpp

gaia_network_asyncdispatcher.o: ../src/gaia_network_asyncdispatcher.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_network_asyncdispatcher.cpp

gaia_network_asyncsocket.o: ../src/gaia_network_asyncsocket.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_network_asyncsocket.cpp

gaia_network_client.o: ../src/gaia_network_client.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_network_client.cpp

gaia_network_http.o: ../src/gaia_network_http.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_network_http.cpp

gaia_network_httpbase.o: ../src/gaia_network_httpbase.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_network_httpbase.cpp

gaia_network_httpserver.o: ../src/gaia_network_httpserver.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_network_httpserver.cpp

gaia_network_server.o: ../src/gaia_network_server.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_network_server.cpp

gaia_network_sudpsocket.o: ../src/gaia_network_sudpsocket.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_network_sudpsocket.cpp

gaia_stream_stdstream.o: ../src/gaia_stream_stdstream.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_stream_stdstream.cpp

gaia_type.o: ../src/gaia_type.cpp
	$(CPPC) $(CPPCFLAGS) ../src/gaia_type.cpp

gzclose.o: ../src/gzclose.c
	$(CC) $(CPPCFLAGS) ../src/gzclose.c

gzlib.o: ../src/gzlib.c
	$(CC) $(CPPCFLAGS) ../src/gzlib.c

gzread.o: ../src/gzread.c
	$(CC) $(CPPCFLAGS) ../src/gzread.c

gzwrite.o: ../src/gzwrite.c
	$(CC) $(CPPCFLAGS) ../src/gzwrite.c

hpdf_3dmeasure.o: ../src/hpdf_3dmeasure.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_3dmeasure.c

hpdf_annotation.o: ../src/hpdf_annotation.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_annotation.c

hpdf_array.o: ../src/hpdf_array.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_array.c

hpdf_binary.o: ../src/hpdf_binary.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_binary.c

hpdf_boolean.o: ../src/hpdf_boolean.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_boolean.c

hpdf_catalog.o: ../src/hpdf_catalog.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_catalog.c

hpdf_destination.o: ../src/hpdf_destination.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_destination.c

hpdf_dict.o: ../src/hpdf_dict.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_dict.c

hpdf_doc_png.o: ../src/hpdf_doc_png.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_doc_png.c

hpdf_doc.o: ../src/hpdf_doc.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_doc.c

hpdf_encoder_cns.o: ../src/hpdf_encoder_cns.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_encoder_cns.c

hpdf_encoder_cnt.o: ../src/hpdf_encoder_cnt.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_encoder_cnt.c

hpdf_encoder_jp.o: ../src/hpdf_encoder_jp.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_encoder_jp.c

hpdf_encoder_kr.o: ../src/hpdf_encoder_kr.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_encoder_kr.c

hpdf_encoder_utf.o: ../src/hpdf_encoder_utf.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_encoder_utf.c

hpdf_encoder.o: ../src/hpdf_encoder.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_encoder.c

hpdf_encrypt.o: ../src/hpdf_encrypt.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_encrypt.c

hpdf_encryptdict.o: ../src/hpdf_encryptdict.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_encryptdict.c

hpdf_error.o: ../src/hpdf_error.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_error.c

hpdf_exdata.o: ../src/hpdf_exdata.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_exdata.c

hpdf_ext_gstate.o: ../src/hpdf_ext_gstate.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_ext_gstate.c

hpdf_font_cid.o: ../src/hpdf_font_cid.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_font_cid.c

hpdf_font_tt.o: ../src/hpdf_font_tt.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_font_tt.c

hpdf_font_type1.o: ../src/hpdf_font_type1.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_font_type1.c

hpdf_font.o: ../src/hpdf_font.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_font.c

hpdf_fontdef_base14.o: ../src/hpdf_fontdef_base14.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_fontdef_base14.c

hpdf_fontdef_cid.o: ../src/hpdf_fontdef_cid.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_fontdef_cid.c

hpdf_fontdef_cns.o: ../src/hpdf_fontdef_cns.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_fontdef_cns.c

hpdf_fontdef_cnt.o: ../src/hpdf_fontdef_cnt.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_fontdef_cnt.c

hpdf_fontdef_jp.o: ../src/hpdf_fontdef_jp.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_fontdef_jp.c

hpdf_fontdef_kr.o: ../src/hpdf_fontdef_kr.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_fontdef_kr.c

hpdf_fontdef_tt.o: ../src/hpdf_fontdef_tt.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_fontdef_tt.c

hpdf_fontdef_type1.o: ../src/hpdf_fontdef_type1.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_fontdef_type1.c

hpdf_fontdef.o: ../src/hpdf_fontdef.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_fontdef.c

hpdf_gstate.o: ../src/hpdf_gstate.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_gstate.c

hpdf_image_ccitt.o: ../src/hpdf_image_ccitt.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_image_ccitt.c

hpdf_image_png.o: ../src/hpdf_image_png.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_image_png.c

hpdf_image.o: ../src/hpdf_image.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_image.c

hpdf_info.o: ../src/hpdf_info.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_info.c

hpdf_list.o: ../src/hpdf_list.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_list.c

hpdf_mmgr.o: ../src/hpdf_mmgr.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_mmgr.c

hpdf_name.o: ../src/hpdf_name.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_name.c

hpdf_namedict.o: ../src/hpdf_namedict.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_namedict.c

hpdf_null.o: ../src/hpdf_null.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_null.c

hpdf_number.o: ../src/hpdf_number.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_number.c

hpdf_objects.o: ../src/hpdf_objects.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_objects.c

hpdf_outline.o: ../src/hpdf_outline.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_outline.c

hpdf_page_label.o: ../src/hpdf_page_label.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_page_label.c

hpdf_page_operator.o: ../src/hpdf_page_operator.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_page_operator.c

hpdf_pages.o: ../src/hpdf_pages.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_pages.c

hpdf_pdfa.o: ../src/hpdf_pdfa.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_pdfa.c

hpdf_real.o: ../src/hpdf_real.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_real.c

hpdf_streams.o: ../src/hpdf_streams.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_streams.c

hpdf_string.o: ../src/hpdf_string.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_string.c

hpdf_u3d.o: ../src/hpdf_u3d.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_u3d.c

hpdf_utils.o: ../src/hpdf_utils.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_utils.c

hpdf_xref.o: ../src/hpdf_xref.c
	$(CC) $(CPPCFLAGS) ../src/hpdf_xref.c

infback.o: ../src/infback.c
	$(CC) $(CPPCFLAGS) ../src/infback.c

inffast.o: ../src/inffast.c
	$(CC) $(CPPCFLAGS) ../src/inffast.c

inflate.o: ../src/inflate.c
	$(CC) $(CPPCFLAGS) ../src/inflate.c

inftrees.o: ../src/inftrees.c
	$(CC) $(CPPCFLAGS) ../src/inftrees.c

pcre_byte_order.o: ../src/pcre_byte_order.c
	$(CC) $(CPPCFLAGS) ../src/pcre_byte_order.c

pcre_chartables.o: ../src/pcre_chartables.c
	$(CC) $(CPPCFLAGS) ../src/pcre_chartables.c

pcre_compile.o: ../src/pcre_compile.c
	$(CC) $(CPPCFLAGS) ../src/pcre_compile.c

pcre_config.o: ../src/pcre_config.c
	$(CC) $(CPPCFLAGS) ../src/pcre_config.c

pcre_dfa_exec.o: ../src/pcre_dfa_exec.c
	$(CC) $(CPPCFLAGS) ../src/pcre_dfa_exec.c

pcre_exec.o: ../src/pcre_exec.c
	$(CC) $(CPPCFLAGS) ../src/pcre_exec.c

pcre_fullinfo.o: ../src/pcre_fullinfo.c
	$(CC) $(CPPCFLAGS) ../src/pcre_fullinfo.c

pcre_get.o: ../src/pcre_get.c
	$(CC) $(CPPCFLAGS) ../src/pcre_get.c

pcre_globals.o: ../src/pcre_globals.c
	$(CC) $(CPPCFLAGS) ../src/pcre_globals.c

pcre_jit_compile.o: ../src/pcre_jit_compile.c
	$(CC) $(CPPCFLAGS) ../src/pcre_jit_compile.c

pcre_maketables.o: ../src/pcre_maketables.c
	$(CC) $(CPPCFLAGS) ../src/pcre_maketables.c

pcre_newline.o: ../src/pcre_newline.c
	$(CC) $(CPPCFLAGS) ../src/pcre_newline.c

pcre_ord2utf8.o: ../src/pcre_ord2utf8.c
	$(CC) $(CPPCFLAGS) ../src/pcre_ord2utf8.c

pcre_printint.o: ../src/pcre_printint.c
	$(CC) $(CPPCFLAGS) ../src/pcre_printint.c

pcre_refcount.o: ../src/pcre_refcount.c
	$(CC) $(CPPCFLAGS) ../src/pcre_refcount.c

pcre_string_utils.o: ../src/pcre_string_utils.c
	$(CC) $(CPPCFLAGS) ../src/pcre_string_utils.c

pcre_study.o: ../src/pcre_study.c
	$(CC) $(CPPCFLAGS) ../src/pcre_study.c

pcre_tables.o: ../src/pcre_tables.c
	$(CC) $(CPPCFLAGS) ../src/pcre_tables.c

pcre_ucd.o: ../src/pcre_ucd.c
	$(CC) $(CPPCFLAGS) ../src/pcre_ucd.c

pcre_valid_utf8.o: ../src/pcre_valid_utf8.c
	$(CC) $(CPPCFLAGS) ../src/pcre_valid_utf8.c

pcre_version.o: ../src/pcre_version.c
	$(CC) $(CPPCFLAGS) ../src/pcre_version.c

pcre_xclass.o: ../src/pcre_xclass.c
	$(CC) $(CPPCFLAGS) ../src/pcre_xclass.c

pcre16_byte_order.o: ../src/pcre16_byte_order.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_byte_order.c

pcre16_chartables.o: ../src/pcre16_chartables.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_chartables.c

pcre16_compile.o: ../src/pcre16_compile.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_compile.c

pcre16_config.o: ../src/pcre16_config.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_config.c

pcre16_dfa_exec.o: ../src/pcre16_dfa_exec.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_dfa_exec.c

pcre16_exec.o: ../src/pcre16_exec.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_exec.c

pcre16_fullinfo.o: ../src/pcre16_fullinfo.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_fullinfo.c

pcre16_get.o: ../src/pcre16_get.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_get.c

pcre16_globals.o: ../src/pcre16_globals.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_globals.c

pcre16_jit_compile.o: ../src/pcre16_jit_compile.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_jit_compile.c

pcre16_maketables.o: ../src/pcre16_maketables.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_maketables.c

pcre16_newline.o: ../src/pcre16_newline.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_newline.c

pcre16_ord2utf16.o: ../src/pcre16_ord2utf16.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_ord2utf16.c

pcre16_printint.o: ../src/pcre16_printint.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_printint.c

pcre16_refcount.o: ../src/pcre16_refcount.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_refcount.c

pcre16_string_utils.o: ../src/pcre16_string_utils.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_string_utils.c

pcre16_study.o: ../src/pcre16_study.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_study.c

pcre16_tables.o: ../src/pcre16_tables.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_tables.c

pcre16_ucd.o: ../src/pcre16_ucd.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_ucd.c

pcre16_utf16_utils.o: ../src/pcre16_utf16_utils.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_utf16_utils.c

pcre16_valid_utf16.o: ../src/pcre16_valid_utf16.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_valid_utf16.c

pcre16_version.o: ../src/pcre16_version.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_version.c

pcre16_xclass.o: ../src/pcre16_xclass.c
	$(CC) $(CPPCFLAGS) ../src/pcre16_xclass.c

pcre32_byte_order.o: ../src/pcre32_byte_order.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_byte_order.c

pcre32_chartables.o: ../src/pcre32_chartables.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_chartables.c

pcre32_compile.o: ../src/pcre32_compile.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_compile.c

pcre32_config.o: ../src/pcre32_config.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_config.c

pcre32_dfa_exec.o: ../src/pcre32_dfa_exec.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_dfa_exec.c

pcre32_exec.o: ../src/pcre32_exec.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_exec.c

pcre32_fullinfo.o: ../src/pcre32_fullinfo.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_fullinfo.c

pcre32_get.o: ../src/pcre32_get.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_get.c

pcre32_globals.o: ../src/pcre32_globals.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_globals.c

pcre32_jit_compile.o: ../src/pcre32_jit_compile.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_jit_compile.c

pcre32_maketables.o: ../src/pcre32_maketables.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_maketables.c

pcre32_newline.o: ../src/pcre32_newline.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_newline.c

pcre32_ord2utf32.o: ../src/pcre32_ord2utf32.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_ord2utf32.c

pcre32_printint.o: ../src/pcre32_printint.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_printint.c

pcre32_refcount.o: ../src/pcre32_refcount.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_refcount.c

pcre32_string_utils.o: ../src/pcre32_string_utils.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_string_utils.c

pcre32_study.o: ../src/pcre32_study.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_study.c

pcre32_tables.o: ../src/pcre32_tables.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_tables.c

pcre32_ucd.o: ../src/pcre32_ucd.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_ucd.c

pcre32_utf32_utils.o: ../src/pcre32_utf32_utils.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_utf32_utils.c

pcre32_valid_utf32.o: ../src/pcre32_valid_utf32.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_valid_utf32.c

pcre32_version.o: ../src/pcre32_version.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_version.c

pcre32_xclass.o: ../src/pcre32_xclass.c
	$(CC) $(CPPCFLAGS) ../src/pcre32_xclass.c

png.o: ../src/png.c
	$(CC) $(CPPCFLAGS) ../src/png.c

pngerror.o: ../src/pngerror.c
	$(CC) $(CPPCFLAGS) ../src/pngerror.c

pngget.o: ../src/pngget.c
	$(CC) $(CPPCFLAGS) ../src/pngget.c

pngmem.o: ../src/pngmem.c
	$(CC) $(CPPCFLAGS) ../src/pngmem.c

pngpread.o: ../src/pngpread.c
	$(CC) $(CPPCFLAGS) ../src/pngpread.c

pngread.o: ../src/pngread.c
	$(CC) $(CPPCFLAGS) ../src/pngread.c

pngrio.o: ../src/pngrio.c
	$(CC) $(CPPCFLAGS) ../src/pngrio.c

pngrtran.o: ../src/pngrtran.c
	$(CC) $(CPPCFLAGS) ../src/pngrtran.c

pngrutil.o: ../src/pngrutil.c
	$(CC) $(CPPCFLAGS) ../src/pngrutil.c

pngset.o: ../src/pngset.c
	$(CC) $(CPPCFLAGS) ../src/pngset.c

pngtrans.o: ../src/pngtrans.c
	$(CC) $(CPPCFLAGS) ../src/pngtrans.c

pngwio.o: ../src/pngwio.c
	$(CC) $(CPPCFLAGS) ../src/pngwio.c

pngwrite.o: ../src/pngwrite.c
	$(CC) $(CPPCFLAGS) ../src/pngwrite.c

pngwtran.o: ../src/pngwtran.c
	$(CC) $(CPPCFLAGS) ../src/pngwtran.c

pngwutil.o: ../src/pngwutil.c
	$(CC) $(CPPCFLAGS) ../src/pngwutil.c

rsa.o: ../src/rsa.c
	$(CC) $(CPPCFLAGS) ../src/rsa.c

sqlite3.o: ../src/sqlite3.c
	$(CC) $(CPPCFLAGS) ../src/sqlite3.c

trees.o: ../src/trees.c
	$(CC) $(CPPCFLAGS) ../src/trees.c

uncompr.o: ../src/uncompr.c
	$(CC) $(CPPCFLAGS) ../src/uncompr.c

zutil.o: ../src/zutil.c
	$(CC) $(CPPCFLAGS) ../src/zutil.c


# Clean.
.PHONY: clean
clean:
	-rm adler32.o
	-rm bignum.o
	-rm compress.o
	-rm crc32.o
	-rm deflate.o
	-rm gaia_db_dblocal.o
	-rm gaia_db_dbremote.o
	-rm gaia_digit_diff.o
	-rm gaia_digit_regexp.o
	-rm gaia_digit_zip.o
	-rm gaia_doc_pdf.o
	-rm gaia_img_base.o
	-rm gaia_img_jpeg.o
	-rm gaia_img_png.o
	-rm gaia_network_asyncdispatcher.o
	-rm gaia_network_asyncsocket.o
	-rm gaia_network_client.o
	-rm gaia_network_http.o
	-rm gaia_network_httpbase.o
	-rm gaia_network_httpserver.o
	-rm gaia_network_server.o
	-rm gaia_network_sudpsocket.o
	-rm gaia_stream_stdstream.o
	-rm gaia_type.o
	-rm gzclose.o
	-rm gzlib.o
	-rm gzread.o
	-rm gzwrite.o
	-rm hpdf_3dmeasure.o
	-rm hpdf_annotation.o
	-rm hpdf_array.o
	-rm hpdf_binary.o
	-rm hpdf_boolean.o
	-rm hpdf_catalog.o
	-rm hpdf_destination.o
	-rm hpdf_dict.o
	-rm hpdf_doc_png.o
	-rm hpdf_doc.o
	-rm hpdf_encoder_cns.o
	-rm hpdf_encoder_cnt.o
	-rm hpdf_encoder_jp.o
	-rm hpdf_encoder_kr.o
	-rm hpdf_encoder_utf.o
	-rm hpdf_encoder.o
	-rm hpdf_encrypt.o
	-rm hpdf_encryptdict.o
	-rm hpdf_error.o
	-rm hpdf_exdata.o
	-rm hpdf_ext_gstate.o
	-rm hpdf_font_cid.o
	-rm hpdf_font_tt.o
	-rm hpdf_font_type1.o
	-rm hpdf_font.o
	-rm hpdf_fontdef_base14.o
	-rm hpdf_fontdef_cid.o
	-rm hpdf_fontdef_cns.o
	-rm hpdf_fontdef_cnt.o
	-rm hpdf_fontdef_jp.o
	-rm hpdf_fontdef_kr.o
	-rm hpdf_fontdef_tt.o
	-rm hpdf_fontdef_type1.o
	-rm hpdf_fontdef.o
	-rm hpdf_gstate.o
	-rm hpdf_image_ccitt.o
	-rm hpdf_image_png.o
	-rm hpdf_image.o
	-rm hpdf_info.o
	-rm hpdf_list.o
	-rm hpdf_mmgr.o
	-rm hpdf_name.o
	-rm hpdf_namedict.o
	-rm hpdf_null.o
	-rm hpdf_number.o
	-rm hpdf_objects.o
	-rm hpdf_outline.o
	-rm hpdf_page_label.o
	-rm hpdf_page_operator.o
	-rm hpdf_pages.o
	-rm hpdf_pdfa.o
	-rm hpdf_real.o
	-rm hpdf_streams.o
	-rm hpdf_string.o
	-rm hpdf_u3d.o
	-rm hpdf_utils.o
	-rm hpdf_xref.o
	-rm infback.o
	-rm inffast.o
	-rm inflate.o
	-rm inftrees.o
	-rm pcre_byte_order.o
	-rm pcre_chartables.o
	-rm pcre_compile.o
	-rm pcre_config.o
	-rm pcre_dfa_exec.o
	-rm pcre_exec.o
	-rm pcre_fullinfo.o
	-rm pcre_get.o
	-rm pcre_globals.o
	-rm pcre_jit_compile.o
	-rm pcre_maketables.o
	-rm pcre_newline.o
	-rm pcre_ord2utf8.o
	-rm pcre_printint.o
	-rm pcre_refcount.o
	-rm pcre_string_utils.o
	-rm pcre_study.o
	-rm pcre_tables.o
	-rm pcre_ucd.o
	-rm pcre_valid_utf8.o
	-rm pcre_version.o
	-rm pcre_xclass.o
	-rm pcre16_byte_order.o
	-rm pcre16_chartables.o
	-rm pcre16_compile.o
	-rm pcre16_config.o
	-rm pcre16_dfa_exec.o
	-rm pcre16_exec.o
	-rm pcre16_fullinfo.o
	-rm pcre16_get.o
	-rm pcre16_globals.o
	-rm pcre16_jit_compile.o
	-rm pcre16_maketables.o
	-rm pcre16_newline.o
	-rm pcre16_ord2utf16.o
	-rm pcre16_printint.o
	-rm pcre16_refcount.o
	-rm pcre16_string_utils.o
	-rm pcre16_study.o
	-rm pcre16_tables.o
	-rm pcre16_ucd.o
	-rm pcre16_utf16_utils.o
	-rm pcre16_valid_utf16.o
	-rm pcre16_version.o
	-rm pcre16_xclass.o
	-rm pcre32_byte_order.o
	-rm pcre32_chartables.o
	-rm pcre32_compile.o
	-rm pcre32_config.o
	-rm pcre32_dfa_exec.o
	-rm pcre32_exec.o
	-rm pcre32_fullinfo.o
	-rm pcre32_get.o
	-rm pcre32_globals.o
	-rm pcre32_jit_compile.o
	-rm pcre32_maketables.o
	-rm pcre32_newline.o
	-rm pcre32_ord2utf32.o
	-rm pcre32_printint.o
	-rm pcre32_refcount.o
	-rm pcre32_string_utils.o
	-rm pcre32_study.o
	-rm pcre32_tables.o
	-rm pcre32_ucd.o
	-rm pcre32_utf32_utils.o
	-rm pcre32_valid_utf32.o
	-rm pcre32_version.o
	-rm pcre32_xclass.o
	-rm png.o
	-rm pngerror.o
	-rm pngget.o
	-rm pngmem.o
	-rm pngpread.o
	-rm pngread.o
	-rm pngrio.o
	-rm pngrtran.o
	-rm pngrutil.o
	-rm pngset.o
	-rm pngtrans.o
	-rm pngwio.o
	-rm pngwrite.o
	-rm pngwtran.o
	-rm pngwutil.o
	-rm rsa.o
	-rm sqlite3.o
	-rm trees.o
	-rm uncompr.o
	-rm zutil.o 
	-rm gaia


