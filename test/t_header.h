#ifndef		__T_GAIA_HEADER_H__
#define		__T_GAIA_HEADER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_msys_leakdetector.h"
#include "gaia_log.h"
#include "t_common.h"

namespace TEST
{
	/* GAIA test proc. */
	extern GAIA::GVOID tp_compiler(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID tp_objstatus(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_type(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_math_basic(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_math_lerp(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_math_random(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_math_argb(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_math_nrgb(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_math_vec2(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_math_vec3(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_math_vec4(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_math_mtx22(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_math_mtx33(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_math_mtx32(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_math_mtx44(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_math_mtx43(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_math_qua(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_math_aabb(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_math_aabr(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_math_lint(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_algo_base(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_algo_compare(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_algo_sort(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_algo_search(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_algo_replace(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_algo_unique(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_algo_hash(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_algo_set(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_algo_string(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_algo_memory(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_iterator(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_ctn_vari(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_bufferfl(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_buffer(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_bufferrw(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_stackbuffer(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_msg(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_ptr(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_ref(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_array(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_vector(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_arrayvector(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_stackstack(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_stack(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_stackqueue(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_queue(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_singlelist(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_doublelist(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_list(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_chars(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_string(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_stringref(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_charsstring(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_stackbitset(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_bitset(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_trietree(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_avltree(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_set(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_map(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_hashset(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_hashmap(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_tree(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_orderless(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_book(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_graph(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_dmpgraph(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_pool(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_storage(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_secset(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_accesser(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_stringpool(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_staticstringpool(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_stringptrpool(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_staticstringptrpool(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_bufferpool(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_staticbufferpool(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_ctn_cooperate(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_stream_stringstream(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_time_time(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_time_timer(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_dbg_perfcollector(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_sync_atomic(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_sync_event(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_sync_lock(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_sync_mutex(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_thread_thread(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_thread_threadpool(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_fsys_file(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_fsys_memfile(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_fsys_dir(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_xml_xml(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_html_html(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_json_json(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_log_log(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_db_local(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_db_remote(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_network_base(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_network_socket(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_network_sudpsocket(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_network_asyncsocket(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_network_httpbase(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_network_httpserver(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_network_http(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_digit_regexp(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_digit_base64(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_digit_rc4(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_digit_rsa(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_digit_md5(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_digit_sha1(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_digit_diff(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_digit_zip(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_locale(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_img_jpeg(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_img_png(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_img_image(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_doc_pdf(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_namespace(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_test(GAIA::LOG::Log& logobj);

	extern GAIA::GVOID t_misc_cmdparam(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID t_misc_cmdline(GAIA::LOG::Log& logobj);

	GINL GAIA::GVOID t_all(GAIA::LOG::Log& logobj)
	{
		GAIA::BL bOutputTime = GAIA::False;
		GAIA::U64 uTimeBegin = 0, uTimeEnd = 0;

		logobj << "GAIA VERSION : " << GAIA_VERSION_STRING << logobj.End();
		logobj << "GAIA COMPILE TIME : " << GAIA_VERSION_COMPILEDATE << " " << GAIA_VERSION_COMPILETIME << logobj.End();
		logobj << "GAIA LAST MODIFIED TIME : " << GAIA_VERSION_LASTMODIFYTIME << logobj.End();
		logobj << "\n" << logobj.End();

		// Every test procedure.
		TTEXT("[GAIA PLATFORM TEST BEGIN]");
		{
			TITEM("Platform: Compiler test begin!"); tp_compiler(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Platform: ObjStatus test begin!"); tp_objstatus(logobj); TITEM("End"); TTEXT("\t");
		}
		TTEXT("[GAIA PLATFORM TEST END]\n");

		TTEXT("[GAIA UNIT TEST BEGIN]");
		{
			TITEM("Type: Type test begin!"); t_type(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Math: Basic test begin!"); t_math_basic(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Math: Lerp test begin!"); t_math_lerp(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Math: Random test begin!"); t_math_random(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Math: ARGB test begin!"); t_math_argb(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Math: NRGB test begin!"); t_math_nrgb(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Math: AABB test begin!"); t_math_aabb(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Math: AABR test begin!"); t_math_aabr(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Math: VEC2 test begin!"); t_math_vec2(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Math: VEC3 test begin!"); t_math_vec3(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Math: VEC4 test begin!"); t_math_vec4(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Math: MTX22 test begin!"); t_math_mtx22(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Math: MTX33 test begin!"); t_math_mtx33(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Math: MTX32 test begin!"); t_math_mtx32(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Math: MTX44 test begin!"); t_math_mtx44(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Math: MTX43 test begin!"); t_math_mtx43(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Math: QUA test begin!"); t_math_qua(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Math: LInt test begin!"); t_math_lint(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Algorithm: Base test begin!"); t_algo_base(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Algorithm: Compare test begin!"); t_algo_compare(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Algorithm: Sort test begin!"); t_algo_sort(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Algorithm: Search test begin!"); t_algo_search(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Algorithm: Replace test begin!"); t_algo_replace(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Algorithm: Unique test begin!"); t_algo_unique(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Algorithm: Hash test begin!"); t_algo_hash(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Algorithm: Set test begin!"); t_algo_set(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Algorithm: String test begin!"); t_algo_string(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Algorithm: Memory test begin!"); t_algo_memory(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Iterator: Iterator test begin!"); t_iterator(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Container: Vari test begin!"); t_ctn_vari(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: BufferFL test begin!"); t_ctn_bufferfl(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Buffer test begin!"); t_ctn_buffer(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: BufferRW test begin!"); t_ctn_bufferrw(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: StackBuffer test begin!"); t_ctn_stackbuffer(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Msg test begin!"); t_ctn_msg(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Ptr test begin!"); t_ctn_ptr(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Ref test begin!"); t_ctn_ref(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Array test begin!"); t_ctn_array(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Vector test begin!"); t_ctn_vector(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: ArrayVector test begin!"); t_ctn_arrayvector(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: StackStack test begin!"); t_ctn_stackstack(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Stack test begin!"); t_ctn_stack(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: StackQueue test begin!"); t_ctn_stackqueue(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Queue test begin!"); t_ctn_queue(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: SingleList test begin!"); t_ctn_singlelist(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: DoubleList test begin!"); t_ctn_doublelist(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: List test begin!"); t_ctn_list(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Chars test begin!"); t_ctn_chars(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: String test begin!"); t_ctn_string(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: StringRef test begin!"); t_ctn_stringref(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: CharsString test begin!"); t_ctn_charsstring(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: StackBitset test begin!"); t_ctn_stackbitset(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Bitset test begin!"); t_ctn_bitset(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: TrieTree test begin!"); t_ctn_trietree(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: AVLTree test begin!"); t_ctn_avltree(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Set test begin!"); t_ctn_set(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Map test begin!"); t_ctn_map(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: HashSet test begin!"); t_ctn_hashset(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: HashMap test begin!"); t_ctn_hashmap(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Tree test begin!"); t_ctn_tree(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Orderless test begin!"); t_ctn_orderless(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Book test begin!"); t_ctn_book(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Pool test begin!"); t_ctn_pool(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Graph test begin!"); t_ctn_graph(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: DMPGraph test begin!"); t_ctn_dmpgraph(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Storage test begin!"); t_ctn_storage(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Secset test begin!"); t_ctn_secset(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Accesser test begin!"); t_ctn_accesser(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: StringPool test begin!"); t_ctn_stringpool(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: StaticStringPool test begin!"); t_ctn_staticstringpool(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: StringPtrPool test begin!"); t_ctn_stringptrpool(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: StaticStringPtrPool test begin!"); t_ctn_staticstringptrpool(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: BufferPool test begin!"); t_ctn_bufferpool(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: StaticBufferPool test begin!"); t_ctn_staticbufferpool(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: Cooperate test begin!"); t_ctn_cooperate(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Stream: StringStream test begin!"); t_stream_stringstream(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Time: Time test begin!"); t_time_time(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Time: Timer test begin!"); t_time_timer(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Dbg: PerfCollector test begin!"); t_dbg_perfcollector(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Sync: Atomic test begin!"); t_sync_atomic(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Sync: Event test begin!"); t_sync_event(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Sync: Lock test begin!"); t_sync_lock(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Sync: Mutex test begin!"); t_sync_mutex(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Thread: Thread test begin!"); t_thread_thread(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Thread: ThreadPool test begin!"); t_thread_threadpool(logobj); TITEM("End"); TTEXT("\t");
			TITEM("File: File test begin!"); t_fsys_file(logobj); TITEM("End"); TTEXT("\t");
			TITEM("File: MemFile test begin!"); t_fsys_memfile(logobj); TITEM("End"); TTEXT("\t");
			TITEM("File: Dir test begin!"); t_fsys_dir(logobj); TITEM("End"); TTEXT("\t");
			TITEM("File: XML test begin!"); t_xml_xml(logobj); TITEM("End"); TTEXT("\t");
			TITEM("File: HTML test begin!"); t_html_html(logobj); TITEM("End"); TTEXT("\t");
			TITEM("File: Json test begin!"); t_json_json(logobj); TITEM("End"); TTEXT("\t");
			
			TITEM("Log: Log test begin!"); t_log_log(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Network: DBLocal test begin!"); t_db_local(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Network: DBRemote test begin!"); t_db_remote(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Network: Network base test begin!"); t_network_base(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Network: Network socket test begin!"); t_network_socket(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Network: Network sudpsocket test begin!"); t_network_sudpsocket(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Network: Network async socket test begin!"); t_network_asyncsocket(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Network: Network http base test begin!"); t_network_httpbase(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Network: Network http server test begin!"); t_network_httpserver(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Network: Network http test begin!"); t_network_http(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Digit: RegExp test begin!"); t_digit_regexp(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Digit: Base64 test begin!"); t_digit_base64(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Digit: RC4 test begin!"); t_digit_rc4(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Digit: RSA test begin!"); t_digit_rsa(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Digit: MD5 test begin!"); t_digit_md5(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Digit: SHA1 test begin!"); t_digit_sha1(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Digit: Diff test begin!"); t_digit_diff(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Digit: Zip test begin!"); t_digit_zip(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Locale test begin!"); t_locale(logobj); TITEM("End"); TTEXT("\t");

			TITEM("JPEG test begin!"); t_img_jpeg(logobj); TITEM("End"); TTEXT("\t");
			TITEM("PNG test begin!"); t_img_png(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Image test begin!"); t_img_image(logobj); TITEM("End"); TTEXT("\t");

			TITEM("PDF test begin!"); t_doc_pdf(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Namespace test begin!"); t_namespace(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Test test begin!"); t_test(logobj); TITEM("End"); TTEXT("\t");

			TITEM("CmdParam test begin!"); t_misc_cmdparam(logobj); TITEM("End"); TTEXT("\t");
			TITEM("CmdLine test begin!"); t_misc_cmdline(logobj); TITEM("End"); TTEXT("\t");
		}
		TTEXT("[GAIA UNIT TEST END]");
	}

	// GAIA performance test proc.
	extern GAIA::GVOID tperf_msys_heapesg(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID tperf_ctn(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID tperf_ctn_avltree(GAIA::LOG::Log& logobj);
	extern GAIA::GVOID tperf_ctn_dmpgraph(GAIA::LOG::Log& logobj);

	GINL GAIA::GVOID t_all_perf(GAIA::LOG::Log& logobj)
	{
		GAIA::BL bOutputTime = GAIA::False;
		GAIA::U64 uTimeBegin = 0, uTimeEnd = 0;

		// Every test procedure.
		TTEXT("[GAIA PERF TEST BEGIN]");
		{
			TITEM("MSys: HeapESG test begin!"); tperf_msys_heapesg(logobj); TITEM("End"); TTEXT("\t");

			TITEM("Container: Ctn perf test begin!"); tperf_ctn(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: AVLTree perf test begin!"); tperf_ctn_avltree(logobj); TITEM("End"); TTEXT("\t");
			TITEM("Container: DmpGraph perf test begin!"); tperf_ctn_dmpgraph(logobj); TITEM("End"); TTEXT("\t");
		}
		TTEXT("[GAIA PERF TEST END]");
	}

	class TestLogCallBack : public GAIA::LOG::Log::CallBack
	{
	public:
		GINL TestLogCallBack()
		{
			this->Reset();
			m_file.Open(_T("test_result.txt"), GAIA::FSYS::File::OPEN_TYPE_WRITE | GAIA::FSYS::File::OPEN_TYPE_CREATEALWAYS);
		}

		GINL GAIA::GVOID Reset()
		{
			m_sLogCount = 0;
			m_sWarningCount = 0;
			m_sErrorCount = 0;
		}

		GINL GAIA::NUM GetLogCount() const{return m_sLogCount;}
		GINL GAIA::NUM GetWarningCount() const{return m_sWarningCount;}
		GINL GAIA::NUM GetErrorCount() const{return m_sErrorCount;}

		virtual GAIA::BL WriteLog(
			GAIA::LOG::Log& logobj,
			const GAIA::TIME::Time& logtime,
			GAIA::LOG::Log::TYPE type,
			GAIA::LOG::Log::__FilterType userfilter,
			const GAIA::TCH* pszLog)
		{
			if(type == GAIA::LOG::Log::TYPE_LOG)
				++m_sLogCount;
			else if(type == GAIA::LOG::Log::TYPE_WARNING)
				++m_sWarningCount;
			else if(type == GAIA::LOG::Log::TYPE_ERROR)
				++m_sErrorCount;

			m_stdstream << pszLog << logobj.GetLineBreak();
			m_file.WriteText(pszLog);
			m_file.WriteText(logobj.GetLineBreak());
			return GAIA::True;
		}
		virtual GAIA::BL FlushLog(GAIA::LOG::Log& logobj)
		{
			m_stdstream.flush();
			m_file.Flush();
			return GAIA::True;
		}

	private:
		GAIA::NUM m_sLogCount;
		GAIA::NUM m_sWarningCount;
		GAIA::NUM m_sErrorCount;
		GAIA::STREAM::STDStream m_stdstream;
		GAIA::FSYS::File m_file;
	};

	GAIA::GVOID test(GAIA::LOG::Log::CallBack* pLogCallBack = GNIL)
	{
		{
			/* Set log callback. */
			TestLogCallBack test_log_cb;
			GAIA::LOG::Log logobj;
			if(pLogCallBack == GNIL)
				logobj.SetCallBack(&test_log_cb);
			else
				logobj.SetCallBack(pLogCallBack);

			GAIA::NUM sTotalErrorCount = 0;
			GAIA::NUM sTotalWarningCount = 0;

			/* Normal test case. */
			{
				test_log_cb.Reset();
				TEST::t_all(logobj);
				if(pLogCallBack == GNIL)
				{
					sTotalErrorCount += test_log_cb.GetErrorCount();
					sTotalWarningCount += test_log_cb.GetWarningCount();
					logobj << "Normal test : Error = " << test_log_cb.GetErrorCount() <<
						", Warning = " << test_log_cb.GetWarningCount() << "\n" << logobj.End();
				}
			}

			/* Performance test case. */
			{
				test_log_cb.Reset();
				GAIA::U64 uPerfTimeBegin = GAIA::TIME::tick_time();
				TEST::t_all_perf(logobj);
				GAIA::U64 uPerfTimeEnd = GAIA::TIME::tick_time();
				logobj << "Time lost = " << (uPerfTimeEnd - uPerfTimeBegin) / 1000 << "(MS)" << logobj.End();
				if(pLogCallBack == GNIL)
				{
					sTotalErrorCount += test_log_cb.GetErrorCount();
					sTotalWarningCount += test_log_cb.GetWarningCount();
					logobj << "Performance test : Error = " << test_log_cb.GetErrorCount() <<
						", Warning = " << test_log_cb.GetWarningCount() << "\n" << logobj.End();
				}
			}

			logobj << "Total Error = " << sTotalErrorCount << ", Warning = " << sTotalWarningCount << "\n" << logobj.End();
		}

		/* Dump leak result. */
	#ifdef GAIA_DEBUG_MEMORYLEAK
		gaia_reset_global_variables();
		g_gaia_leakdetector.dump();
	#endif
	}
}

#endif
