#ifndef		__GAIA_H__
#define		__GAIA_H__

/* Include file header. */
#include	"gaia_version.h"
#include	"gaia_sysconfig.h"
#include	"gaia_macrohelper.h"
#include	"gaia_predecl.h"
#include	"gaia_type.h"
#include	"gaia_exception.h"

#include	"gaia_stream_format.h"
#include	"gaia_stream_streambase.h"
#include	"gaia_stream_stdstream.h"

#include	"gaia_assert.h"
#include	"gaia_type_traits2.h"

#include	"gaia_platform.h"
#include	"gaia_internal.h"

#include	"gaia_type_auto.h"

#include	"gaia_system.h"

#include	"gaia_thread_base.h"
#include	"gaia_sync_base.h"
#include	"gaia_sync_atomic.h"
#include	"gaia_sync_lock.h"
#include 	"gaia_sync_autolock.h"
#include	"gaia_sync_lockpure.h"
#include	"gaia_sync_autolockpure.h"
#include	"gaia_sync_lockfree.h"
#include	"gaia_sync_autolockfree.h"
#include	"gaia_sync_lockrw.h"
#include	"gaia_sync_autolockr.h"
#include	"gaia_sync_autolockw.h"
#include	"gaia_sync_event.h"
#include	"gaia_sync_mutex.h"

#include	"gaia_locale.h"

#include	"gaia_math_constant.h"
#include	"gaia_math_base.h"
#include	"gaia_math_convert.h"
#include 	"gaia_math_random.h"
#include	"gaia_math_random_lcg.h"
#include 	"gaia_math_lerp.h"
#include	"gaia_math_linearalgebra.h"
#include	"gaia_math_distance.h"
#include	"gaia_math_limit.h"
#include	"gaia_math_geo.h"
#include	"gaia_math_intersection.h"
#include	"gaia_math_transform.h"
#include	"gaia_math_projection.h"
#include	"gaia_math_noise.h"
#include	"gaia_math_spline.h"
#include	"gaia_math_cull.h"

#include	"gaia_physics_constant.h"

#include	"gaia_msys_base.h"
#include	"gaia_msys_heapcrt.h"
#include	"gaia_msys_heapesg.h"
#include	"gaia_msys_leakdetector.h"

#include	"gaia_iterator.h"

#include	"gaia_algo_base.h"
#include	"gaia_algo_compare.h"
#include	"gaia_algo_memory.h"
#include	"gaia_algo_extend.h"
#include	"gaia_algo_string.h"
#include 	"gaia_algo_search.h"
#include 	"gaia_algo_sort.h"
#include 	"gaia_algo_unique.h"
#include	"gaia_algo_replace.h"
#include	"gaia_algo_set.h"
#include	"gaia_algo_hash.h"
#include	"gaia_algo_hashlcg.h"

#include	"gaia_ctn.h"
#include 	"gaia_ctn_datarecord.h"
#include 	"gaia_ctn_ref.h"
#include	"gaia_ctn_ptr.h"
#include	"gaia_ctn_pair.h"
#include 	"gaia_ctn_stackbitset.h"
#include 	"gaia_ctn_bitset.h"
#include 	"gaia_ctn_chars.h"
#include	"gaia_ctn_string.h"
#include	"gaia_ctn_stringref.h"
#include	"gaia_ctn_charsstring.h"
#include 	"gaia_ctn_stackstack.h"
#include	"gaia_ctn_stack.h"
#include	"gaia_ctn_array.h"
#include	"gaia_ctn_vector.h"
#include	"gaia_ctn_arrayvector.h"
#include	"gaia_ctn_stackqueue.h"
#include	"gaia_ctn_queue.h"
#include 	"gaia_ctn_bufferfl.h"
#include	"gaia_ctn_buffer.h"
#include	"gaia_ctn_stackbuffer.h"
#include	"gaia_ctn_msg.h"
#include	"gaia_ctn_orderless.h"
#include 	"gaia_ctn_pool.h"
#include	"gaia_ctn_book.h"
#include	"gaia_ctn_singlelist.h"
#include	"gaia_ctn_doublelist.h"
#include 	"gaia_ctn_list.h"
#include 	"gaia_ctn_bufferrw.h"
#include 	"gaia_ctn_avltree.h"
#include 	"gaia_ctn_trietree.h"
#include 	"gaia_ctn_kdtree.h"
#include 	"gaia_ctn_ksvdtree.h"
#include 	"gaia_ctn_tree.h"
#include 	"gaia_ctn_priqueue.h"
#include 	"gaia_ctn_set.h"
#include	"gaia_ctn_map.h"
#include	"gaia_ctn_hashset.h"
#include	"gaia_ctn_hashmap.h"
#include	"gaia_ctn_graph.h"
#include	"gaia_ctn_dmpgraph.h"
#include 	"gaia_ctn_net.h"
#include	"gaia_ctn_dictionary.h"
#include	"gaia_ctn_heap.h"
#include 	"gaia_ctn_stringpool.h"
#include 	"gaia_ctn_staticstringpool.h"
#include	"gaia_ctn_stringptrpool.h"
#include	"gaia_ctn_staticstringptrpool.h"
#include	"gaia_ctn_bufferpool.h"
#include	"gaia_ctn_staticbufferpool.h"
#include 	"gaia_ctn_vari.h"
#include	"gaia_ctn_storage.h"
#include 	"gaia_ctn_secset.h"
#include	"gaia_ctn_cooperation.h"

#include	"gaia_math_cal.h"
#include 	"gaia_math_lint.h"
#include 	"gaia_math_lreal.h"
#include 	"gaia_math_lcomplex.h"

#include	"gaia_stream_stringstream.h"

#include	"gaia_time.h"
#include	"gaia_timer_timer.h"
#include	"gaia_timer_realtimer.h"

#include	"gaia_thread.h"
#include	"gaia_thread_pool.h"
#include	"gaia_process.h"

#include 	"gaia_math_rid.h"
#include	"gaia_math_coord.h"

#include	"gaia_math_argb.h"
#include	"gaia_math_nrgb.h"
#include	"gaia_math_vec2.h"
#include	"gaia_math_vec3.h"
#include	"gaia_math_vec4.h"
#include	"gaia_math_qua.h"
#include	"gaia_math_mtx22.h"
#include	"gaia_math_mtx32.h"
#include	"gaia_math_mtx33.h"
#include	"gaia_math_mtx43.h"
#include	"gaia_math_mtx44.h"
#include	"gaia_math_plane.h"
#include	"gaia_math_aabr.h"
#include	"gaia_math_aabb.h"
#include	"gaia_math_cooperation.h"

#include	"gaia_fsys_filebase.h"
#include	"gaia_fsys_dirbase.h"
#include	"gaia_fsys_file.h"
#include	"gaia_fsys_memfile.h"
#include	"gaia_fsys_dir.h"

#include	"gaia_log.h"

#include	"gaia_dbg_perf.h"

#include	"gaia_ctn_accesser.h"

#include 	"gaia_digit_crc.h"
#include	"gaia_digit_base64.h"
#include	"gaia_digit_md5.h"
#include	"gaia_digit_sha1.h"
#include 	"gaia_digit_regexp.h"
#include	"gaia_digit_crypt.h"
#include	"gaia_digit_rc4.h"
#include	"gaia_digit_des.h"
#include	"gaia_digit_aes.h"
#include	"gaia_digit_rsa.h"
#include	"gaia_digit_zip.h"
#include	"gaia_digit_diff.h"

#include 	"gaia_xml_xmlbase.h"
#include	"gaia_xml_xmlreader.h"
#include	"gaia_xml_xmlwriter.h"
#include 	"gaia_xml_xmlfactorydesc.h"
#include 	"gaia_xml_xmlnode.h"
#include 	"gaia_xml_xml.h"
#include 	"gaia_xml_xmlfactory.h"

#include 	"gaia_html_htmlbase.h"
#include 	"gaia_html_htmlfactorydesc.h"
#include 	"gaia_html_htmlnode.h"
#include 	"gaia_html_html.h"
#include 	"gaia_html_htmlfactory.h"

#include	"gaia_json_jsonbase.h"
#include	"gaia_json_jsonreader.h"
#include	"gaia_json_jsonwriter.h"
#include	"gaia_json_jsonfactorydesc.h"
#include	"gaia_json_jsonnode.h"
#include	"gaia_json_json.h"
#include	"gaia_json_jsonfactory.h"

#include	"gaia_network_ip.h"
#include	"gaia_network_addr.h"
#include	"gaia_network_base.h"
#include 	"gaia_network_socket.h"
#include	"gaia_network_asyncsocket.h"
#include	"gaia_network_asyncdispatcher.h"
#include	"gaia_network_sudpsocket.h"
#include	"gaia_network_server.h"
#include	"gaia_network_client.h"
#include	"gaia_network_httpbase.h"
#include	"gaia_network_httpserver.h"
#include	"gaia_network_http.h"

#include 	"gaia_db_dblocal.h"
#include	"gaia_db_dbremote.h"

#include	"gaia_img_base.h"
#include	"gaia_img_jpeg.h"
#include	"gaia_img_png.h"
#include	"gaia_img_image.h"

#include	"gaia_doc_pdf.h"

#include	"gaia_test_context.h"
#include	"gaia_test_unit.h"
#include	"gaia_test_case.h"
#include	"gaia_test_controller.h"

#include	"gaia_global_decl.h"

#include	"gaia_misc_cmdparam.h"
#include	"gaia_misc_cmdline.h"

#include	"gaia_type_impl.h"
#include	"gaia_timer_timer_impl.h"
#include	"gaia_thread_base_impl.h"
#include	"gaia_assert_impl.h"
#include 	"gaia_xml_xmlnode_impl.h"
#include 	"gaia_xml_xml_impl.h"
#include 	"gaia_xml_xmlfactory_impl.h"
#include 	"gaia_html_htmlnode_impl.h"
#include 	"gaia_html_html_impl.h"
#include 	"gaia_html_htmlfactory_impl.h"
#include	"gaia_json_jsonnode_impl.h"
#include	"gaia_json_json_impl.h"
#include	"gaia_json_jsonfactory_impl.h"
#include	"gaia_network_base_impl.h"
#include 	"gaia_network_socket_impl.h"
#include	"gaia_test_unit_impl.h"
#include	"gaia_test_case_impl.h"
#include	"gaia_math_rid_impl.h"

#endif

/*
*
*    HOW TO INCLUDE THE GAIA's HEADER ?
*
*    The following header files will been included by gaia's user manually.
*    The user will include these header files by his choose.
*
*    [gaia_global_impl.h]
*        gaia is designed as a header file based source code
*        library, so gaia not exist any global variables. but unfortunately,
*        gaia need some basic global variable, for example : global-msys.
*        So, if you want use this function, include this header file after
*        include gaia.h, or will change gaia's source code for compile errors.
*
*    [gaia_using_namespace.h]
*        include this file will cause all gaia namespace been used.
*        WARNING! gaia's namespace will protect gaia source code
*        library's symbol been declaration as unique symbol. if you
*        want to implement a high quality program, please NOT include
*        this header file(RECOMMANDED).
*
*/

/*
*
*    THIS IS THE CLASS DIAGRAM.
*
*    [BASIC CLASS DIAGRAM]
*
*         |- All derived class will been allocate by HIGH-PERFORMANCE heap.
*         |  All simple class will derived here. Like class XxxxDesc and etc.
*         |
*         |              |- Support abstract statistics and optimize interface.
*         |              |
*         |              |               |- Not used(Reserved).
*         |              |               |
*         |              |               |                |- Support thread-safe reference count.
*         |              |               |                |
*         |              |               |                |
*         |              |               |                |
*         Base -------> Entity -------> Object -------> RefObject
*
*
*
*/

/*!
	@mainpage Manual
	@section sec_introduction Introduction
		@subsection Feature
		@subsection Setup
	@section sec_structure Structure
	@section sec_reference Reference
		@subsection API reference
	@section sec_sample Sample
	@section sec_qanda Q & A
*/
