# Constants declarations.
CC=gcc
CPPC=g++
CCFLAGS=-O2 -I../include/ -DINVALID -Wall -c
CPPCFLAGS=-O2 -I../include/ -DINVALID -Wall -c
LINKFLAGS=-liconv -o


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
	zutil.o \
	preheader.o \
	t_algo_base.o \
	t_algo_compare.o \
	t_algo_hash.o \
	t_algo_memory.o \
	t_algo_replace.o \
	t_algo_search.o \
	t_algo_set.o \
	t_algo_sort.o \
	t_algo_string.o \
	t_algo_unique.o \
	t_ctn_accesser.o \
	t_ctn_array.o \
	t_ctn_arrayvector.o \
	t_ctn_avltree.o \
	t_ctn_bitset.o \
	t_ctn_book.o \
	t_ctn_buffer.o \
	t_ctn_bufferfl.o \
	t_ctn_bufferpool.o \
	t_ctn_bufferrw.o \
	t_ctn_chars.o \
	t_ctn_charsstring.o \
	t_ctn_cooperate.o \
	t_ctn_dmpgraph.o \
	t_ctn_doublelist.o \
	t_ctn_graph.o \
	t_ctn_hashmap.o \
	t_ctn_hashset.o \
	t_ctn_list.o \
	t_ctn_map.o \
	t_ctn_msg.o \
	t_ctn_orderless.o \
	t_ctn_pool.o \
	t_ctn_ptr.o \
	t_ctn_queue.o \
	t_ctn_ref.o \
	t_ctn_secset.o \
	t_ctn_set.o \
	t_ctn_singlelist.o \
	t_ctn_stack.o \
	t_ctn_stackbitset.o \
	t_ctn_stackbuffer.o \
	t_ctn_stackqueue.o \
	t_ctn_stackstack.o \
	t_ctn_staticbufferpool.o \
	t_ctn_staticstringpool.o \
	t_ctn_staticstringptrpool.o \
	t_ctn_storage.o \
	t_ctn_string.o \
	t_ctn_stringpool.o \
	t_ctn_stringptrpool.o \
	t_ctn_stringref.o \
	t_ctn_tree.o \
	t_ctn_trietree.o \
	t_ctn_vari.o \
	t_ctn_vector.o \
	t_db_local.o \
	t_db_remote.o \
	t_dbg_objwatcher.o \
	t_dbg_perfcollector.o \
	t_digit_aes.o \
	t_digit_base64.o \
	t_digit_crc.o \
	t_digit_diff.o \
	t_digit_md5.o \
	t_digit_percentage.o \
	t_digit_rc4.o \
	t_digit_regexp.o \
	t_digit_rsa.o \
	t_digit_sha1.o \
	t_digit_zip.o \
	t_doc_pdf.o \
	t_fsys_dir.o \
	t_fsys_file.o \
	t_fsys_memfile.o \
	t_html_html.o \
	t_img_image.o \
	t_img_jpeg.o \
	t_img_png.o \
	t_iterator.o \
	t_json_json.o \
	t_json_jsonreader.o \
	t_json_jsonwriter.o \
	t_locale_language.o \
	t_locale.o \
	t_log_log.o \
	t_math_aabb.o \
	t_math_aabr.o \
	t_math_argb.o \
	t_math_basic.o \
	t_math_lerp.o \
	t_math_lint.o \
	t_math_mtx22.o \
	t_math_mtx32.o \
	t_math_mtx33.o \
	t_math_mtx43.o \
	t_math_mtx44.o \
	t_math_nrgb.o \
	t_math_qua.o \
	t_math_random.o \
	t_math_vec2.o \
	t_math_vec3.o \
	t_math_vec4.o \
	t_misc_cmdline.o \
	t_misc_cmdparam.o \
	t_namespace.o \
	t_network_asyncsocket.o \
	t_network_base.o \
	t_network_http.o \
	t_network_httpbase.o \
	t_network_httpserver.o \
	t_network_socket.o \
	t_network_sudpsocket.o \
	t_pattern_stringindex.o \
	t_pattern_textindex.o \
	t_stream_stringstream.o \
	t_sync_atomic.o \
	t_sync_event.o \
	t_sync_lock.o \
	t_sync_mutex.o \
	t_test.o \
	t_thread_thread.o \
	t_thread_threadpool.o \
	t_time_time.o \
	t_time_timer.o \
	t_type.o \
	t_xml_xml.o \
	t_xml_xmlreader.o \
	t_xml_xmlwriter.o \
	tp_compiler.o \
	tp_objstatus.o \
	tperf_ctn_avltree.o \
	tperf_ctn_dmpgraph.o \
	tperf_ctn.o \
	tperf_msys_heapesg.o \
	main.o

# Link
link: $(objects)
	$(CPPC) $(objects) $(LINKFLAGS) gaia


# Every objects
adler32.o: ../src/adler32.c
	$(CC) $(CCFLAGS) ../src/adler32.c

bignum.o: ../src/bignum.c
	$(CC) $(CCFLAGS) ../src/bignum.c

compress.o: ../src/compress.c
	$(CC) $(CCFLAGS) ../src/compress.c

crc32.o: ../src/crc32.c
	$(CC) $(CCFLAGS) ../src/crc32.c

deflate.o: ../src/deflate.c
	$(CC) $(CCFLAGS) ../src/deflate.c

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
	$(CC) $(CCFLAGS) ../src/gzclose.c

gzlib.o: ../src/gzlib.c
	$(CC) $(CCFLAGS) ../src/gzlib.c

gzread.o: ../src/gzread.c
	$(CC) $(CCFLAGS) ../src/gzread.c

gzwrite.o: ../src/gzwrite.c
	$(CC) $(CCFLAGS) ../src/gzwrite.c

hpdf_3dmeasure.o: ../src/hpdf_3dmeasure.c
	$(CC) $(CCFLAGS) ../src/hpdf_3dmeasure.c

hpdf_annotation.o: ../src/hpdf_annotation.c
	$(CC) $(CCFLAGS) ../src/hpdf_annotation.c

hpdf_array.o: ../src/hpdf_array.c
	$(CC) $(CCFLAGS) ../src/hpdf_array.c

hpdf_binary.o: ../src/hpdf_binary.c
	$(CC) $(CCFLAGS) ../src/hpdf_binary.c

hpdf_boolean.o: ../src/hpdf_boolean.c
	$(CC) $(CCFLAGS) ../src/hpdf_boolean.c

hpdf_catalog.o: ../src/hpdf_catalog.c
	$(CC) $(CCFLAGS) ../src/hpdf_catalog.c

hpdf_destination.o: ../src/hpdf_destination.c
	$(CC) $(CCFLAGS) ../src/hpdf_destination.c

hpdf_dict.o: ../src/hpdf_dict.c
	$(CC) $(CCFLAGS) ../src/hpdf_dict.c

hpdf_doc_png.o: ../src/hpdf_doc_png.c
	$(CC) $(CCFLAGS) ../src/hpdf_doc_png.c

hpdf_doc.o: ../src/hpdf_doc.c
	$(CC) $(CCFLAGS) ../src/hpdf_doc.c

hpdf_encoder_cns.o: ../src/hpdf_encoder_cns.c
	$(CC) $(CCFLAGS) ../src/hpdf_encoder_cns.c

hpdf_encoder_cnt.o: ../src/hpdf_encoder_cnt.c
	$(CC) $(CCFLAGS) ../src/hpdf_encoder_cnt.c

hpdf_encoder_jp.o: ../src/hpdf_encoder_jp.c
	$(CC) $(CCFLAGS) ../src/hpdf_encoder_jp.c

hpdf_encoder_kr.o: ../src/hpdf_encoder_kr.c
	$(CC) $(CCFLAGS) ../src/hpdf_encoder_kr.c

hpdf_encoder_utf.o: ../src/hpdf_encoder_utf.c
	$(CC) $(CCFLAGS) ../src/hpdf_encoder_utf.c

hpdf_encoder.o: ../src/hpdf_encoder.c
	$(CC) $(CCFLAGS) ../src/hpdf_encoder.c

hpdf_encrypt.o: ../src/hpdf_encrypt.c
	$(CC) $(CCFLAGS) ../src/hpdf_encrypt.c

hpdf_encryptdict.o: ../src/hpdf_encryptdict.c
	$(CC) $(CCFLAGS) ../src/hpdf_encryptdict.c

hpdf_error.o: ../src/hpdf_error.c
	$(CC) $(CCFLAGS) ../src/hpdf_error.c

hpdf_exdata.o: ../src/hpdf_exdata.c
	$(CC) $(CCFLAGS) ../src/hpdf_exdata.c

hpdf_ext_gstate.o: ../src/hpdf_ext_gstate.c
	$(CC) $(CCFLAGS) ../src/hpdf_ext_gstate.c

hpdf_font_cid.o: ../src/hpdf_font_cid.c
	$(CC) $(CCFLAGS) ../src/hpdf_font_cid.c

hpdf_font_tt.o: ../src/hpdf_font_tt.c
	$(CC) $(CCFLAGS) ../src/hpdf_font_tt.c

hpdf_font_type1.o: ../src/hpdf_font_type1.c
	$(CC) $(CCFLAGS) ../src/hpdf_font_type1.c

hpdf_font.o: ../src/hpdf_font.c
	$(CC) $(CCFLAGS) ../src/hpdf_font.c

hpdf_fontdef_base14.o: ../src/hpdf_fontdef_base14.c
	$(CC) $(CCFLAGS) ../src/hpdf_fontdef_base14.c

hpdf_fontdef_cid.o: ../src/hpdf_fontdef_cid.c
	$(CC) $(CCFLAGS) ../src/hpdf_fontdef_cid.c

hpdf_fontdef_cns.o: ../src/hpdf_fontdef_cns.c
	$(CC) $(CCFLAGS) ../src/hpdf_fontdef_cns.c

hpdf_fontdef_cnt.o: ../src/hpdf_fontdef_cnt.c
	$(CC) $(CCFLAGS) ../src/hpdf_fontdef_cnt.c

hpdf_fontdef_jp.o: ../src/hpdf_fontdef_jp.c
	$(CC) $(CCFLAGS) ../src/hpdf_fontdef_jp.c

hpdf_fontdef_kr.o: ../src/hpdf_fontdef_kr.c
	$(CC) $(CCFLAGS) ../src/hpdf_fontdef_kr.c

hpdf_fontdef_tt.o: ../src/hpdf_fontdef_tt.c
	$(CC) $(CCFLAGS) ../src/hpdf_fontdef_tt.c

hpdf_fontdef_type1.o: ../src/hpdf_fontdef_type1.c
	$(CC) $(CCFLAGS) ../src/hpdf_fontdef_type1.c

hpdf_fontdef.o: ../src/hpdf_fontdef.c
	$(CC) $(CCFLAGS) ../src/hpdf_fontdef.c

hpdf_gstate.o: ../src/hpdf_gstate.c
	$(CC) $(CCFLAGS) ../src/hpdf_gstate.c

hpdf_image_ccitt.o: ../src/hpdf_image_ccitt.c
	$(CC) $(CCFLAGS) ../src/hpdf_image_ccitt.c

hpdf_image_png.o: ../src/hpdf_image_png.c
	$(CC) $(CCFLAGS) ../src/hpdf_image_png.c

hpdf_image.o: ../src/hpdf_image.c
	$(CC) $(CCFLAGS) ../src/hpdf_image.c

hpdf_info.o: ../src/hpdf_info.c
	$(CC) $(CCFLAGS) ../src/hpdf_info.c

hpdf_list.o: ../src/hpdf_list.c
	$(CC) $(CCFLAGS) ../src/hpdf_list.c

hpdf_mmgr.o: ../src/hpdf_mmgr.c
	$(CC) $(CCFLAGS) ../src/hpdf_mmgr.c

hpdf_name.o: ../src/hpdf_name.c
	$(CC) $(CCFLAGS) ../src/hpdf_name.c

hpdf_namedict.o: ../src/hpdf_namedict.c
	$(CC) $(CCFLAGS) ../src/hpdf_namedict.c

hpdf_null.o: ../src/hpdf_null.c
	$(CC) $(CCFLAGS) ../src/hpdf_null.c

hpdf_number.o: ../src/hpdf_number.c
	$(CC) $(CCFLAGS) ../src/hpdf_number.c

hpdf_objects.o: ../src/hpdf_objects.c
	$(CC) $(CCFLAGS) ../src/hpdf_objects.c

hpdf_outline.o: ../src/hpdf_outline.c
	$(CC) $(CCFLAGS) ../src/hpdf_outline.c

hpdf_page_label.o: ../src/hpdf_page_label.c
	$(CC) $(CCFLAGS) ../src/hpdf_page_label.c

hpdf_page_operator.o: ../src/hpdf_page_operator.c
	$(CC) $(CCFLAGS) ../src/hpdf_page_operator.c

hpdf_pages.o: ../src/hpdf_pages.c
	$(CC) $(CCFLAGS) ../src/hpdf_pages.c

hpdf_pdfa.o: ../src/hpdf_pdfa.c
	$(CC) $(CCFLAGS) ../src/hpdf_pdfa.c

hpdf_real.o: ../src/hpdf_real.c
	$(CC) $(CCFLAGS) ../src/hpdf_real.c

hpdf_streams.o: ../src/hpdf_streams.c
	$(CC) $(CCFLAGS) ../src/hpdf_streams.c

hpdf_string.o: ../src/hpdf_string.c
	$(CC) $(CCFLAGS) ../src/hpdf_string.c

hpdf_u3d.o: ../src/hpdf_u3d.c
	$(CC) $(CCFLAGS) ../src/hpdf_u3d.c

hpdf_utils.o: ../src/hpdf_utils.c
	$(CC) $(CCFLAGS) ../src/hpdf_utils.c

hpdf_xref.o: ../src/hpdf_xref.c
	$(CC) $(CCFLAGS) ../src/hpdf_xref.c

infback.o: ../src/infback.c
	$(CC) $(CCFLAGS) ../src/infback.c

inffast.o: ../src/inffast.c
	$(CC) $(CCFLAGS) ../src/inffast.c

inflate.o: ../src/inflate.c
	$(CC) $(CCFLAGS) ../src/inflate.c

inftrees.o: ../src/inftrees.c
	$(CC) $(CCFLAGS) ../src/inftrees.c

pcre_byte_order.o: ../src/pcre_byte_order.c
	$(CC) $(CCFLAGS) ../src/pcre_byte_order.c

pcre_chartables.o: ../src/pcre_chartables.c
	$(CC) $(CCFLAGS) ../src/pcre_chartables.c

pcre_compile.o: ../src/pcre_compile.c
	$(CC) $(CCFLAGS) ../src/pcre_compile.c

pcre_config.o: ../src/pcre_config.c
	$(CC) $(CCFLAGS) ../src/pcre_config.c

pcre_dfa_exec.o: ../src/pcre_dfa_exec.c
	$(CC) $(CCFLAGS) ../src/pcre_dfa_exec.c

pcre_exec.o: ../src/pcre_exec.c
	$(CC) $(CCFLAGS) ../src/pcre_exec.c

pcre_fullinfo.o: ../src/pcre_fullinfo.c
	$(CC) $(CCFLAGS) ../src/pcre_fullinfo.c

pcre_get.o: ../src/pcre_get.c
	$(CC) $(CCFLAGS) ../src/pcre_get.c

pcre_globals.o: ../src/pcre_globals.c
	$(CC) $(CCFLAGS) ../src/pcre_globals.c

pcre_jit_compile.o: ../src/pcre_jit_compile.c
	$(CC) $(CCFLAGS) ../src/pcre_jit_compile.c

pcre_maketables.o: ../src/pcre_maketables.c
	$(CC) $(CCFLAGS) ../src/pcre_maketables.c

pcre_newline.o: ../src/pcre_newline.c
	$(CC) $(CCFLAGS) ../src/pcre_newline.c

pcre_ord2utf8.o: ../src/pcre_ord2utf8.c
	$(CC) $(CCFLAGS) ../src/pcre_ord2utf8.c

pcre_printint.o: ../src/pcre_printint.c
	$(CC) $(CCFLAGS) ../src/pcre_printint.c

pcre_refcount.o: ../src/pcre_refcount.c
	$(CC) $(CCFLAGS) ../src/pcre_refcount.c

pcre_string_utils.o: ../src/pcre_string_utils.c
	$(CC) $(CCFLAGS) ../src/pcre_string_utils.c

pcre_study.o: ../src/pcre_study.c
	$(CC) $(CCFLAGS) ../src/pcre_study.c

pcre_tables.o: ../src/pcre_tables.c
	$(CC) $(CCFLAGS) ../src/pcre_tables.c

pcre_ucd.o: ../src/pcre_ucd.c
	$(CC) $(CCFLAGS) ../src/pcre_ucd.c

pcre_valid_utf8.o: ../src/pcre_valid_utf8.c
	$(CC) $(CCFLAGS) ../src/pcre_valid_utf8.c

pcre_version.o: ../src/pcre_version.c
	$(CC) $(CCFLAGS) ../src/pcre_version.c

pcre_xclass.o: ../src/pcre_xclass.c
	$(CC) $(CCFLAGS) ../src/pcre_xclass.c

pcre16_byte_order.o: ../src/pcre16_byte_order.c
	$(CC) $(CCFLAGS) ../src/pcre16_byte_order.c

pcre16_chartables.o: ../src/pcre16_chartables.c
	$(CC) $(CCFLAGS) ../src/pcre16_chartables.c

pcre16_compile.o: ../src/pcre16_compile.c
	$(CC) $(CCFLAGS) ../src/pcre16_compile.c

pcre16_config.o: ../src/pcre16_config.c
	$(CC) $(CCFLAGS) ../src/pcre16_config.c

pcre16_dfa_exec.o: ../src/pcre16_dfa_exec.c
	$(CC) $(CCFLAGS) ../src/pcre16_dfa_exec.c

pcre16_exec.o: ../src/pcre16_exec.c
	$(CC) $(CCFLAGS) ../src/pcre16_exec.c

pcre16_fullinfo.o: ../src/pcre16_fullinfo.c
	$(CC) $(CCFLAGS) ../src/pcre16_fullinfo.c

pcre16_get.o: ../src/pcre16_get.c
	$(CC) $(CCFLAGS) ../src/pcre16_get.c

pcre16_globals.o: ../src/pcre16_globals.c
	$(CC) $(CCFLAGS) ../src/pcre16_globals.c

pcre16_jit_compile.o: ../src/pcre16_jit_compile.c
	$(CC) $(CCFLAGS) ../src/pcre16_jit_compile.c

pcre16_maketables.o: ../src/pcre16_maketables.c
	$(CC) $(CCFLAGS) ../src/pcre16_maketables.c

pcre16_newline.o: ../src/pcre16_newline.c
	$(CC) $(CCFLAGS) ../src/pcre16_newline.c

pcre16_ord2utf16.o: ../src/pcre16_ord2utf16.c
	$(CC) $(CCFLAGS) ../src/pcre16_ord2utf16.c

pcre16_printint.o: ../src/pcre16_printint.c
	$(CC) $(CCFLAGS) ../src/pcre16_printint.c

pcre16_refcount.o: ../src/pcre16_refcount.c
	$(CC) $(CCFLAGS) ../src/pcre16_refcount.c

pcre16_string_utils.o: ../src/pcre16_string_utils.c
	$(CC) $(CCFLAGS) ../src/pcre16_string_utils.c

pcre16_study.o: ../src/pcre16_study.c
	$(CC) $(CCFLAGS) ../src/pcre16_study.c

pcre16_tables.o: ../src/pcre16_tables.c
	$(CC) $(CCFLAGS) ../src/pcre16_tables.c

pcre16_ucd.o: ../src/pcre16_ucd.c
	$(CC) $(CCFLAGS) ../src/pcre16_ucd.c

pcre16_utf16_utils.o: ../src/pcre16_utf16_utils.c
	$(CC) $(CCFLAGS) ../src/pcre16_utf16_utils.c

pcre16_valid_utf16.o: ../src/pcre16_valid_utf16.c
	$(CC) $(CCFLAGS) ../src/pcre16_valid_utf16.c

pcre16_version.o: ../src/pcre16_version.c
	$(CC) $(CCFLAGS) ../src/pcre16_version.c

pcre16_xclass.o: ../src/pcre16_xclass.c
	$(CC) $(CCFLAGS) ../src/pcre16_xclass.c

pcre32_byte_order.o: ../src/pcre32_byte_order.c
	$(CC) $(CCFLAGS) ../src/pcre32_byte_order.c

pcre32_chartables.o: ../src/pcre32_chartables.c
	$(CC) $(CCFLAGS) ../src/pcre32_chartables.c

pcre32_compile.o: ../src/pcre32_compile.c
	$(CC) $(CCFLAGS) ../src/pcre32_compile.c

pcre32_config.o: ../src/pcre32_config.c
	$(CC) $(CCFLAGS) ../src/pcre32_config.c

pcre32_dfa_exec.o: ../src/pcre32_dfa_exec.c
	$(CC) $(CCFLAGS) ../src/pcre32_dfa_exec.c

pcre32_exec.o: ../src/pcre32_exec.c
	$(CC) $(CCFLAGS) ../src/pcre32_exec.c

pcre32_fullinfo.o: ../src/pcre32_fullinfo.c
	$(CC) $(CCFLAGS) ../src/pcre32_fullinfo.c

pcre32_get.o: ../src/pcre32_get.c
	$(CC) $(CCFLAGS) ../src/pcre32_get.c

pcre32_globals.o: ../src/pcre32_globals.c
	$(CC) $(CCFLAGS) ../src/pcre32_globals.c

pcre32_jit_compile.o: ../src/pcre32_jit_compile.c
	$(CC) $(CCFLAGS) ../src/pcre32_jit_compile.c

pcre32_maketables.o: ../src/pcre32_maketables.c
	$(CC) $(CCFLAGS) ../src/pcre32_maketables.c

pcre32_newline.o: ../src/pcre32_newline.c
	$(CC) $(CCFLAGS) ../src/pcre32_newline.c

pcre32_ord2utf32.o: ../src/pcre32_ord2utf32.c
	$(CC) $(CCFLAGS) ../src/pcre32_ord2utf32.c

pcre32_printint.o: ../src/pcre32_printint.c
	$(CC) $(CCFLAGS) ../src/pcre32_printint.c

pcre32_refcount.o: ../src/pcre32_refcount.c
	$(CC) $(CCFLAGS) ../src/pcre32_refcount.c

pcre32_string_utils.o: ../src/pcre32_string_utils.c
	$(CC) $(CCFLAGS) ../src/pcre32_string_utils.c

pcre32_study.o: ../src/pcre32_study.c
	$(CC) $(CCFLAGS) ../src/pcre32_study.c

pcre32_tables.o: ../src/pcre32_tables.c
	$(CC) $(CCFLAGS) ../src/pcre32_tables.c

pcre32_ucd.o: ../src/pcre32_ucd.c
	$(CC) $(CCFLAGS) ../src/pcre32_ucd.c

pcre32_utf32_utils.o: ../src/pcre32_utf32_utils.c
	$(CC) $(CCFLAGS) ../src/pcre32_utf32_utils.c

pcre32_valid_utf32.o: ../src/pcre32_valid_utf32.c
	$(CC) $(CCFLAGS) ../src/pcre32_valid_utf32.c

pcre32_version.o: ../src/pcre32_version.c
	$(CC) $(CCFLAGS) ../src/pcre32_version.c

pcre32_xclass.o: ../src/pcre32_xclass.c
	$(CC) $(CCFLAGS) ../src/pcre32_xclass.c

png.o: ../src/png.c
	$(CC) $(CCFLAGS) ../src/png.c

pngerror.o: ../src/pngerror.c
	$(CC) $(CCFLAGS) ../src/pngerror.c

pngget.o: ../src/pngget.c
	$(CC) $(CCFLAGS) ../src/pngget.c

pngmem.o: ../src/pngmem.c
	$(CC) $(CCFLAGS) ../src/pngmem.c

pngpread.o: ../src/pngpread.c
	$(CC) $(CCFLAGS) ../src/pngpread.c

pngread.o: ../src/pngread.c
	$(CC) $(CCFLAGS) ../src/pngread.c

pngrio.o: ../src/pngrio.c
	$(CC) $(CCFLAGS) ../src/pngrio.c

pngrtran.o: ../src/pngrtran.c
	$(CC) $(CCFLAGS) ../src/pngrtran.c

pngrutil.o: ../src/pngrutil.c
	$(CC) $(CCFLAGS) ../src/pngrutil.c

pngset.o: ../src/pngset.c
	$(CC) $(CCFLAGS) ../src/pngset.c

pngtrans.o: ../src/pngtrans.c
	$(CC) $(CCFLAGS) ../src/pngtrans.c

pngwio.o: ../src/pngwio.c
	$(CC) $(CCFLAGS) ../src/pngwio.c

pngwrite.o: ../src/pngwrite.c
	$(CC) $(CCFLAGS) ../src/pngwrite.c

pngwtran.o: ../src/pngwtran.c
	$(CC) $(CCFLAGS) ../src/pngwtran.c

pngwutil.o: ../src/pngwutil.c
	$(CC) $(CCFLAGS) ../src/pngwutil.c

rsa.o: ../src/rsa.c
	$(CC) $(CCFLAGS) ../src/rsa.c

sqlite3.o: ../src/sqlite3.c
	$(CC) $(CCFLAGS) ../src/sqlite3.c

trees.o: ../src/trees.c
	$(CC) $(CCFLAGS) ../src/trees.c

uncompr.o: ../src/uncompr.c
	$(CC) $(CCFLAGS) ../src/uncompr.c

zutil.o: ../src/zutil.c
	$(CC) $(CCFLAGS) ../src/zutil.c

preheader.o: ../test/preheader.cpp
	$(CPPC) $(CPPCFLAGS) ../test/preheader.cpp

t_algo_base.o: ../test/t_algo_base.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_algo_base.cpp

t_algo_compare.o: ../test/t_algo_compare.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_algo_compare.cpp

t_algo_hash.o: ../test/t_algo_hash.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_algo_hash.cpp

t_algo_memory.o: ../test/t_algo_memory.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_algo_memory.cpp

t_algo_replace.o: ../test/t_algo_replace.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_algo_replace.cpp

t_algo_search.o: ../test/t_algo_search.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_algo_search.cpp

t_algo_set.o: ../test/t_algo_set.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_algo_set.cpp

t_algo_sort.o: ../test/t_algo_sort.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_algo_sort.cpp

t_algo_string.o: ../test/t_algo_string.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_algo_string.cpp

t_algo_unique.o: ../test/t_algo_unique.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_algo_unique.cpp

t_ctn_accesser.o: ../test/t_ctn_accesser.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_accesser.cpp

t_ctn_array.o: ../test/t_ctn_array.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_array.cpp

t_ctn_arrayvector.o: ../test/t_ctn_arrayvector.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_arrayvector.cpp

t_ctn_avltree.o: ../test/t_ctn_avltree.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_avltree.cpp

t_ctn_bitset.o: ../test/t_ctn_bitset.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_bitset.cpp

t_ctn_book.o: ../test/t_ctn_book.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_book.cpp

t_ctn_buffer.o: ../test/t_ctn_buffer.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_buffer.cpp

t_ctn_bufferfl.o: ../test/t_ctn_bufferfl.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_bufferfl.cpp

t_ctn_bufferpool.o: ../test/t_ctn_bufferpool.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_bufferpool.cpp

t_ctn_bufferrw.o: ../test/t_ctn_bufferrw.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_bufferrw.cpp

t_ctn_chars.o: ../test/t_ctn_chars.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_chars.cpp

t_ctn_charsstring.o: ../test/t_ctn_charsstring.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_charsstring.cpp

t_ctn_cooperate.o: ../test/t_ctn_cooperate.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_cooperate.cpp

t_ctn_dmpgraph.o: ../test/t_ctn_dmpgraph.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_dmpgraph.cpp

t_ctn_doublelist.o: ../test/t_ctn_doublelist.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_doublelist.cpp

t_ctn_graph.o: ../test/t_ctn_graph.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_graph.cpp

t_ctn_hashmap.o: ../test/t_ctn_hashmap.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_hashmap.cpp

t_ctn_hashset.o: ../test/t_ctn_hashset.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_hashset.cpp

t_ctn_list.o: ../test/t_ctn_list.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_list.cpp

t_ctn_map.o: ../test/t_ctn_map.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_map.cpp

t_ctn_msg.o: ../test/t_ctn_msg.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_msg.cpp

t_ctn_orderless.o: ../test/t_ctn_orderless.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_orderless.cpp

t_ctn_pool.o: ../test/t_ctn_pool.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_pool.cpp

t_ctn_ptr.o: ../test/t_ctn_ptr.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_ptr.cpp

t_ctn_queue.o: ../test/t_ctn_queue.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_queue.cpp

t_ctn_ref.o: ../test/t_ctn_ref.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_ref.cpp

t_ctn_secset.o: ../test/t_ctn_secset.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_secset.cpp

t_ctn_set.o: ../test/t_ctn_set.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_set.cpp

t_ctn_singlelist.o: ../test/t_ctn_singlelist.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_singlelist.cpp

t_ctn_stack.o: ../test/t_ctn_stack.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_stack.cpp

t_ctn_stackbitset.o: ../test/t_ctn_stackbitset.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_stackbitset.cpp

t_ctn_stackbuffer.o: ../test/t_ctn_stackbuffer.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_stackbuffer.cpp

t_ctn_stackqueue.o: ../test/t_ctn_stackqueue.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_stackqueue.cpp

t_ctn_stackstack.o: ../test/t_ctn_stackstack.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_stackstack.cpp

t_ctn_staticbufferpool.o: ../test/t_ctn_staticbufferpool.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_staticbufferpool.cpp

t_ctn_staticstringpool.o: ../test/t_ctn_staticstringpool.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_staticstringpool.cpp

t_ctn_staticstringptrpool.o: ../test/t_ctn_staticstringptrpool.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_staticstringptrpool.cpp

t_ctn_storage.o: ../test/t_ctn_storage.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_storage.cpp

t_ctn_string.o: ../test/t_ctn_string.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_string.cpp

t_ctn_stringpool.o: ../test/t_ctn_stringpool.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_stringpool.cpp

t_ctn_stringptrpool.o: ../test/t_ctn_stringptrpool.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_stringptrpool.cpp

t_ctn_stringref.o: ../test/t_ctn_stringref.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_stringref.cpp

t_ctn_tree.o: ../test/t_ctn_tree.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_tree.cpp

t_ctn_trietree.o: ../test/t_ctn_trietree.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_trietree.cpp

t_ctn_vari.o: ../test/t_ctn_vari.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_vari.cpp

t_ctn_vector.o: ../test/t_ctn_vector.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_ctn_vector.cpp

t_db_local.o: ../test/t_db_local.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_db_local.cpp

t_db_remote.o: ../test/t_db_remote.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_db_remote.cpp

t_dbg_objwatcher.o: ../test/t_dbg_objwatcher.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_dbg_objwatcher.cpp

t_dbg_perfcollector.o: ../test/t_dbg_perfcollector.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_dbg_perfcollector.cpp

t_digit_aes.o: ../test/t_digit_aes.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_digit_aes.cpp

t_digit_base64.o: ../test/t_digit_base64.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_digit_base64.cpp

t_digit_crc.o: ../test/t_digit_crc.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_digit_crc.cpp

t_digit_diff.o: ../test/t_digit_diff.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_digit_diff.cpp

t_digit_md5.o: ../test/t_digit_md5.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_digit_md5.cpp

t_digit_percentage.o: ../test/t_digit_percentage.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_digit_percentage.cpp

t_digit_rc4.o: ../test/t_digit_rc4.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_digit_rc4.cpp

t_digit_regexp.o: ../test/t_digit_regexp.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_digit_regexp.cpp

t_digit_rsa.o: ../test/t_digit_rsa.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_digit_rsa.cpp

t_digit_sha1.o: ../test/t_digit_sha1.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_digit_sha1.cpp

t_digit_zip.o: ../test/t_digit_zip.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_digit_zip.cpp

t_doc_pdf.o: ../test/t_doc_pdf.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_doc_pdf.cpp

t_fsys_dir.o: ../test/t_fsys_dir.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_fsys_dir.cpp

t_fsys_file.o: ../test/t_fsys_file.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_fsys_file.cpp

t_fsys_memfile.o: ../test/t_fsys_memfile.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_fsys_memfile.cpp

t_html_html.o: ../test/t_html_html.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_html_html.cpp

t_img_image.o: ../test/t_img_image.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_img_image.cpp

t_img_jpeg.o: ../test/t_img_jpeg.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_img_jpeg.cpp

t_img_png.o: ../test/t_img_png.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_img_png.cpp

t_iterator.o: ../test/t_iterator.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_iterator.cpp

t_json_json.o: ../test/t_json_json.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_json_json.cpp

t_json_jsonreader.o: ../test/t_json_jsonreader.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_json_jsonreader.cpp

t_json_jsonwriter.o: ../test/t_json_jsonwriter.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_json_jsonwriter.cpp

t_locale_language.o: ../test/t_locale_language.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_locale_language.cpp

t_locale.o: ../test/t_locale.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_locale.cpp

t_log_log.o: ../test/t_log_log.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_log_log.cpp

t_math_aabb.o: ../test/t_math_aabb.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_aabb.cpp

t_math_aabr.o: ../test/t_math_aabr.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_aabr.cpp

t_math_argb.o: ../test/t_math_argb.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_argb.cpp

t_math_basic.o: ../test/t_math_basic.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_basic.cpp

t_math_lerp.o: ../test/t_math_lerp.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_lerp.cpp

t_math_lint.o: ../test/t_math_lint.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_lint.cpp

t_math_mtx22.o: ../test/t_math_mtx22.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_mtx22.cpp

t_math_mtx32.o: ../test/t_math_mtx32.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_mtx32.cpp

t_math_mtx33.o: ../test/t_math_mtx33.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_mtx33.cpp

t_math_mtx43.o: ../test/t_math_mtx43.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_mtx43.cpp

t_math_mtx44.o: ../test/t_math_mtx44.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_mtx44.cpp

t_math_nrgb.o: ../test/t_math_nrgb.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_nrgb.cpp

t_math_qua.o: ../test/t_math_qua.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_qua.cpp

t_math_random.o: ../test/t_math_random.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_random.cpp

t_math_vec2.o: ../test/t_math_vec2.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_vec2.cpp

t_math_vec3.o: ../test/t_math_vec3.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_vec3.cpp

t_math_vec4.o: ../test/t_math_vec4.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_math_vec4.cpp

t_misc_cmdline.o: ../test/t_misc_cmdline.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_misc_cmdline.cpp

t_misc_cmdparam.o: ../test/t_misc_cmdparam.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_misc_cmdparam.cpp

t_namespace.o: ../test/t_namespace.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_namespace.cpp

t_network_asyncsocket.o: ../test/t_network_asyncsocket.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_network_asyncsocket.cpp

t_network_base.o: ../test/t_network_base.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_network_base.cpp

t_network_http.o: ../test/t_network_http.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_network_http.cpp

t_network_httpbase.o: ../test/t_network_httpbase.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_network_httpbase.cpp

t_network_httpserver.o: ../test/t_network_httpserver.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_network_httpserver.cpp

t_network_socket.o: ../test/t_network_socket.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_network_socket.cpp

t_network_sudpsocket.o: ../test/t_network_sudpsocket.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_network_sudpsocket.cpp

t_pattern_stringindex.o: ../test/t_pattern_stringindex.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_pattern_stringindex.cpp

t_pattern_textindex.o: ../test/t_pattern_textindex.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_pattern_textindex.cpp

t_stream_stringstream.o: ../test/t_stream_stringstream.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_stream_stringstream.cpp

t_sync_atomic.o: ../test/t_sync_atomic.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_sync_atomic.cpp

t_sync_event.o: ../test/t_sync_event.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_sync_event.cpp

t_sync_lock.o: ../test/t_sync_lock.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_sync_lock.cpp

t_sync_mutex.o: ../test/t_sync_mutex.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_sync_mutex.cpp

t_test.o: ../test/t_test.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_test.cpp

t_thread_thread.o: ../test/t_thread_thread.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_thread_thread.cpp

t_thread_threadpool.o: ../test/t_thread_threadpool.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_thread_threadpool.cpp

t_time_time.o: ../test/t_time_time.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_time_time.cpp

t_time_timer.o: ../test/t_time_timer.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_time_timer.cpp

t_type.o: ../test/t_type.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_type.cpp

t_xml_xml.o: ../test/t_xml_xml.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_xml_xml.cpp

t_xml_xmlreader.o: ../test/t_xml_xmlreader.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_xml_xmlreader.cpp

t_xml_xmlwriter.o: ../test/t_xml_xmlwriter.cpp
	$(CPPC) $(CPPCFLAGS) ../test/t_xml_xmlwriter.cpp

tp_compiler.o: ../test/tp_compiler.cpp
	$(CPPC) $(CPPCFLAGS) ../test/tp_compiler.cpp

tp_objstatus.o: ../test/tp_objstatus.cpp
	$(CPPC) $(CPPCFLAGS) ../test/tp_objstatus.cpp

tperf_ctn_avltree.o: ../test/tperf_ctn_avltree.cpp
	$(CPPC) $(CPPCFLAGS) ../test/tperf_ctn_avltree.cpp

tperf_ctn_dmpgraph.o: ../test/tperf_ctn_dmpgraph.cpp
	$(CPPC) $(CPPCFLAGS) ../test/tperf_ctn_dmpgraph.cpp

tperf_ctn.o: ../test/tperf_ctn.cpp
	$(CPPC) $(CPPCFLAGS) ../test/tperf_ctn.cpp

tperf_msys_heapesg.o: ../test/tperf_msys_heapesg.cpp
	$(CPPC) $(CPPCFLAGS) ../test/tperf_msys_heapesg.cpp

main.o: main.cpp
	$(CPPC) $(CPPCFLAGS) main.cpp


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
	-rm preheader.o
	-rm t_algo_base.o
	-rm t_algo_compare.o
	-rm t_algo_hash.o
	-rm t_algo_memory.o
	-rm t_algo_replace.o
	-rm t_algo_search.o
	-rm t_algo_set.o
	-rm t_algo_sort.o
	-rm t_algo_string.o
	-rm t_algo_unique.o
	-rm t_ctn_accesser.o
	-rm t_ctn_array.o
	-rm t_ctn_arrayvector.o
	-rm t_ctn_avltree.o
	-rm t_ctn_bitset.o
	-rm t_ctn_book.o
	-rm t_ctn_buffer.o
	-rm t_ctn_bufferfl.o
	-rm t_ctn_bufferpool.o
	-rm t_ctn_bufferrw.o
	-rm t_ctn_chars.o
	-rm t_ctn_charsstring.o
	-rm t_ctn_cooperate.o
	-rm t_ctn_dmpgraph.o
	-rm t_ctn_doublelist.o
	-rm t_ctn_graph.o
	-rm t_ctn_hashmap.o
	-rm t_ctn_hashset.o
	-rm t_ctn_list.o
	-rm t_ctn_map.o
	-rm t_ctn_msg.o
	-rm t_ctn_orderless.o
	-rm t_ctn_pool.o
	-rm t_ctn_ptr.o
	-rm t_ctn_queue.o
	-rm t_ctn_ref.o
	-rm t_ctn_secset.o
	-rm t_ctn_set.o
	-rm t_ctn_singlelist.o
	-rm t_ctn_stack.o
	-rm t_ctn_stackbitset.o
	-rm t_ctn_stackbuffer.o
	-rm t_ctn_stackqueue.o
	-rm t_ctn_stackstack.o
	-rm t_ctn_staticbufferpool.o
	-rm t_ctn_staticstringpool.o
	-rm t_ctn_staticstringptrpool.o
	-rm t_ctn_storage.o
	-rm t_ctn_string.o
	-rm t_ctn_stringpool.o
	-rm t_ctn_stringptrpool.o
	-rm t_ctn_stringref.o
	-rm t_ctn_tree.o
	-rm t_ctn_trietree.o
	-rm t_ctn_vari.o
	-rm t_ctn_vector.o
	-rm t_db_local.o
	-rm t_db_remote.o
	-rm t_dbg_objwatcher.o
	-rm t_dbg_perfcollector.o
	-rm t_digit_aes.o
	-rm t_digit_base64.o
	-rm t_digit_crc.o
	-rm t_digit_diff.o
	-rm t_digit_md5.o
	-rm t_digit_percentage.o
	-rm t_digit_rc4.o
	-rm t_digit_regexp.o
	-rm t_digit_rsa.o
	-rm t_digit_sha1.o
	-rm t_digit_zip.o
	-rm t_doc_pdf.o
	-rm t_fsys_dir.o
	-rm t_fsys_file.o
	-rm t_fsys_memfile.o
	-rm t_html_html.o
	-rm t_img_image.o
	-rm t_img_jpeg.o
	-rm t_img_png.o
	-rm t_iterator.o
	-rm t_json_json.o
	-rm t_json_jsonreader.o
	-rm t_json_jsonwriter.o
	-rm t_locale_language.o
	-rm t_locale.o
	-rm t_log_log.o
	-rm t_math_aabb.o
	-rm t_math_aabr.o
	-rm t_math_argb.o
	-rm t_math_basic.o
	-rm t_math_lerp.o
	-rm t_math_lint.o
	-rm t_math_mtx22.o
	-rm t_math_mtx32.o
	-rm t_math_mtx33.o
	-rm t_math_mtx43.o
	-rm t_math_mtx44.o
	-rm t_math_nrgb.o
	-rm t_math_qua.o
	-rm t_math_random.o
	-rm t_math_vec2.o
	-rm t_math_vec3.o
	-rm t_math_vec4.o
	-rm t_misc_cmdline.o
	-rm t_misc_cmdparam.o
	-rm t_namespace.o
	-rm t_network_asyncsocket.o
	-rm t_network_base.o
	-rm t_network_http.o
	-rm t_network_httpbase.o
	-rm t_network_httpserver.o
	-rm t_network_socket.o
	-rm t_network_sudpsocket.o
	-rm t_pattern_stringindex.o
	-rm t_pattern_textindex.o
	-rm t_stream_stringstream.o
	-rm t_sync_atomic.o
	-rm t_sync_event.o
	-rm t_sync_lock.o
	-rm t_sync_mutex.o
	-rm t_test.o
	-rm t_thread_thread.o
	-rm t_thread_threadpool.o
	-rm t_time_time.o
	-rm t_time_timer.o
	-rm t_type.o
	-rm t_xml_xml.o
	-rm t_xml_xmlreader.o
	-rm t_xml_xmlwriter.o
	-rm tp_compiler.o
	-rm tp_objstatus.o
	-rm tperf_ctn_avltree.o
	-rm tperf_ctn_dmpgraph.o
	-rm tperf_ctn.o
	-rm tperf_msys_heapesg.o
	-rm main.o
	-rm gaia
