#include "preheader.h"
#include "t_common.h"

namespace TEST
{
	extern GAIA::GVOID t_ctn_accesser(GAIA::LOG::Log& logobj)
	{
		static const GAIA::NUM ACCESS_ELEMENT_COUNT = 100;

		/* RAM based accesser test. */
		{
			/* Accesser pointer operator test. */
			{
				typedef GAIA::N16 __DataType;
				typedef GAIA::CTN::Accesser<__DataType, GAIA::NM, GAIA::ALGO::ExtendGold<GAIA::NM> > __AccType;
				__AccType acc;
				__DataType arr[ACCESS_ELEMENT_COUNT];
				TAST(acc.bindmem(arr, ACCESS_ELEMENT_COUNT, __AccType::ACCESS_TYPE_READ | __AccType::ACCESS_TYPE_WRITE));
				TAST(acc.offset(100));
				if(acc.offset() != 100)
					TERROR;
				TAST(acc.stride(sizeof(__DataType) + 1));
				if(acc.stride() != sizeof(__DataType) + 1)
					TERROR;
				TAST(acc.index(0));
				if(acc.index() != 0)
					TERROR;
				__AccType::_datatype t = 0;
				while(!acc.empty())
				{
					*acc = t;
					++t;
					++acc;
				}
				if(t != (ACCESS_ELEMENT_COUNT - 100) / 3)
					TERROR;
				TAST(acc.index(t / 2));
				if(acc.index() != t / 2)
					TERROR;
				while(!acc.empty())
				{
					*acc = -*acc;
					++acc;
				}
				TAST(acc.index(0));
				if(acc.index() != 0)
					TERROR;
				__AccType::_datatype tt = 0;
				while(!acc.empty())
				{
					if(acc.index() >= t / 2)
					{
						if(*acc != -tt)
						{
							TERROR;
							break;
						}
					}
					else
					{
						if(*acc != +tt)
						{
							TERROR;
							break;
						}
					}
					++tt;
					++acc;
				}
				TAST(!acc);
				if(acc.index() != t)
					TERROR;
				acc.index(0);
				__AccType acc1 = acc;
				acc1 += 50;
				if(acc1 - acc != 50)
					TERROR;
				acc1 -= 10;
				acc += 20;
				if(acc1 - acc != 20)
					TERROR;
				if(!(acc1 > acc) ||
					acc1 < acc ||
					acc1 <= acc ||
					acc1 == acc ||
					!(acc1 >= acc) ||
					!(acc1 != acc))
				{
					TERROR;
				}
				__AccType acc2 = acc + 20;
				if(acc2 != acc1)
					TERROR;
			}

			/* Make accesser work as a string pointer. */
			{
				GAIA::TCH szTemp[ACCESS_ELEMENT_COUNT];
				typedef GAIA::CTN::Accesser<GAIA::TCH, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __AccType;
				__AccType acc;
				acc.bindmem(szTemp, ACCESS_ELEMENT_COUNT, __AccType::ACCESS_TYPE_READ | __AccType::ACCESS_TYPE_WRITE);
				GAIA::ALGO::gstrcpy(acc, "HelloWorld");
				if(GAIA::ALGO::gstrcmp(acc, "HelloWorld") != 0)
					TERROR;
			}

			/* Expandable accesser test. */
			{
				typedef GAIA::CTN::Accesser<GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __AccType;
				__AccType acc;
				acc.expandable(GAIA::True);
				acc.bindmem(GNIL, 0, __AccType::ACCESS_TYPE_READ | __AccType::ACCESS_TYPE_WRITE);
				for(GAIA::NUM x = 0; x < 100; ++x)
					acc[x] = x;
				for(GAIA::NUM x = 0; x < 100; ++x)
				{
					if(acc[x] != x)
					{
						TERROR;
						break;
					}
				}
				gdel[] acc.bindmem();
				acc.destroy();
			}

			/* Expandable accesser bubble sort test. */
			//{
			//	typedef GAIA::CTN::Accesser<GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __AccType;
			//	__AccType acc;
			//	acc.expandable(GAIA::True);
			//	acc.bindmem(GNIL, 0, __AccType::ACCESS_TYPE_READ | __AccType::ACCESS_TYPE_WRITE);
			//	for(GAIA::NUM x = 0; x < 100; ++x)
			//		acc[x] = GAIA::MATH::xrandom();
			//	GAIA::ALGO::bsort(acc, acc + 99);
			//	if(!GAIA::ALGO::issorted(acc, acc + 99))
			//	{
			//		TERROR;
			//
			//	}
			//	gdel[] acc.bindmem();
			//	acc.destroy();
			//}

			/* Accesser write read test. */
			{
				GAIA::NUM szTemp[ACCESS_ELEMENT_COUNT];
				typedef GAIA::CTN::Accesser<GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __AccType;
				__AccType acc;
				acc.bindmem(szTemp, ACCESS_ELEMENT_COUNT * sizeof(GAIA::NUM), __AccType::ACCESS_TYPE_READ | __AccType::ACCESS_TYPE_WRITE);
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT / (GAIA::NUM)sizeof(GAIA::NUM); ++x)
				{
					if(acc.write(&x, sizeof(x)) != sizeof(x))
						TERROR;
				}
				acc.index(0);
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT / (GAIA::NUM)sizeof(GAIA::NUM); ++x)
				{
					GAIA::NUM e;
					if(acc.read(&e, sizeof(e)) != sizeof(e))
					{
						TERROR;
						break;
					}
					if(e != x)
					{
						TERROR;
						break;
					}
				}
			}

			/* Accesser write read ovewflow test. */
			{
				GAIA::NUM szTemp[ACCESS_ELEMENT_COUNT];
				typedef GAIA::CTN::Accesser<GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __AccType;
				__AccType acc;
				acc.bindmem(szTemp, ACCESS_ELEMENT_COUNT * sizeof(GAIA::NUM), __AccType::ACCESS_TYPE_READ | __AccType::ACCESS_TYPE_WRITE);
				GAIA::CTN::Vector<GAIA::NUM> listEle;
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT * 2; ++x)
					listEle.push_back(x);
				if(acc.write(listEle.fptr(), listEle.size() * sizeof(GAIA::NUM)) != ACCESS_ELEMENT_COUNT * sizeof(GAIA::NUM))
					TERROR;
				acc.index(0);
				listEle.inverse();
				if(acc.read(listEle.fptr(), listEle.size() * sizeof(GAIA::NUM)) != ACCESS_ELEMENT_COUNT * sizeof(GAIA::NUM))
					TERROR;
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT / (GAIA::NUM)sizeof(GAIA::NUM); ++x)
				{
					if(listEle[x] != x)
					{
						TERROR;
						break;
					}
				}
			}

			/* Expandable accesser read write test. */
			{
				typedef GAIA::CTN::Accesser<GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __AccType;
				__AccType acc;
				acc.bindmem(GNIL, 0, __AccType::ACCESS_TYPE_READ | __AccType::ACCESS_TYPE_WRITE);
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT; ++x)
				{
					if(acc.write(&x, sizeof(x)) != sizeof(x))
					{
						TERROR;
						break;
					}
				}
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT / 10; ++x)
				{
					acc.index(x * 10 + 9);
					GAIA::NUM t = GINVALID;
					if(acc.write(&t, sizeof(t)) != sizeof(t))
					{
						TERROR;
						break;
					}
				}
				acc.index(0);
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT; ++x)
				{
					GAIA::NUM t;
					if(acc.read(&t, sizeof(t)) != sizeof(t))
					{
						TERROR;
						break;
					}
					if((x + 1) % 10 == 0)
					{
						if(t != GINVALID)
						{
							TERROR;
							break;
						}
					}
					else
					{
						if(t != x)
						{
							TERROR;
							break;
						}
					}
				}
				gdel[] acc.bindmem();
				acc.destroy();
			}
		}

		/* File based accesser test. */
		{
			GAIA::CTN::AString strFileName = g_gaia_appdocdir;
			strFileName += "accesser_file";

			/* Accesser pointer operator test. */
			{
				typedef GAIA::N16 __DataType;
				typedef GAIA::CTN::Accesser<__DataType, GAIA::NM, GAIA::ALGO::ExtendGold<GAIA::NM> > __AccType;
				__AccType acc;
				GAIA::FSYS::File accfile;
				if(!accfile.Open(strFileName,
					GAIA::FSYS::File::OPEN_TYPE_READ |
					GAIA::FSYS::File::OPEN_TYPE_WRITE |
					GAIA::FSYS::File::OPEN_TYPE_CREATEALWAYS))
				{
					TERROR;
				}
				accfile.Resize(ACCESS_ELEMENT_COUNT);
				TAST(acc.bindfile(&accfile, __AccType::ACCESS_TYPE_READ | __AccType::ACCESS_TYPE_WRITE));
				TAST(acc.offset(100));
				if(acc.offset() != 100)
					TERROR;
				TAST(acc.stride(sizeof(__DataType) + 1));
				if(acc.stride() != sizeof(__DataType) + 1)
					TERROR;
				TAST(acc.index(0));
				if(acc.index() != 0)
					TERROR;
				__AccType::_datatype t = 0;
				while(!acc.empty())
				{
					*acc = t;
					++t;
					++acc;
				}
				if(t != (ACCESS_ELEMENT_COUNT - 100) / 3)
					TERROR;
				TAST(acc.index(t / 2));
				if(acc.index() != t / 2)
					TERROR;
				while(!acc.empty())
				{
					*acc = -*acc;
					++acc;
				}
				TAST(acc.index(0));
				if(acc.index() != 0)
					TERROR;
				__AccType::_datatype tt = 0;
				while(!acc.empty())
				{
					if(acc.index() >= t / 2)
					{
						if(*acc != -tt)
						{
							TERROR;
							break;
						}
					}
					else
					{
						if(*acc != +tt)
						{
							TERROR;
							break;
						}
					}
					++tt;
					++acc;
				}
				TAST(!acc);
				if(acc.index() != t)
					TERROR;
				acc.index(0);
				__AccType acc1 = acc;
				acc1 += 50;
				if(acc1 - acc != 50)
					TERROR;
				acc1 -= 10;
				acc += 20;
				if(acc1 - acc != 20)
					TERROR;
				if(!(acc1 > acc) ||
					acc1 < acc ||
					acc1 <= acc ||
					acc1 == acc ||
					!(acc1 >= acc) ||
					!(acc1 != acc))
				{
					TERROR;
				}
				__AccType acc2 = acc + 20;
				if(acc2 != acc1)
					TERROR;
			}

			/* Make accesser work as a string pointer. */
			{
				GAIA::FSYS::File accfile;
				if(!accfile.Open(strFileName,
					GAIA::FSYS::File::OPEN_TYPE_READ |
					GAIA::FSYS::File::OPEN_TYPE_WRITE |
					GAIA::FSYS::File::OPEN_TYPE_CREATEALWAYS))
				{
					TERROR;
				}
				accfile.Resize(ACCESS_ELEMENT_COUNT);
				typedef GAIA::CTN::Accesser<GAIA::TCH, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __AccType;
				__AccType acc;
				TAST(acc.bindfile(&accfile, __AccType::ACCESS_TYPE_READ | __AccType::ACCESS_TYPE_WRITE));
				GAIA::ALGO::gstrcpy(acc, "HelloWorld");
				if(GAIA::ALGO::gstrcmp(acc, "HelloWorld") != 0)
					TERROR;
			}

			/* Expandable accesser test. */
			{
				typedef GAIA::CTN::Accesser<GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __AccType;
				__AccType acc;
				acc.expandable(GAIA::True);
				GAIA::CH szNewFileName[GAIA::MAXPL];
				TAST(acc.bindfile(GNIL, __AccType::ACCESS_TYPE_READ | __AccType::ACCESS_TYPE_WRITE, szNewFileName, sizeof(szNewFileName)));
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT; ++x)
					acc[x] = x;
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT; ++x)
				{
					if(acc[x] != x)
					{
						TERROR;
						break;
					}
				}
				gdel acc.bindfile();
				GAIA::FSYS::Dir dir;
				dir.RemoveFile(szNewFileName);
			}

			/* Expandable accesser bubble sort test. */
			//{
			//	typedef GAIA::CTN::Accesser<GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __AccType;
			//	__AccType acc;
			//	acc.expandable(GAIA::True);
			//	GAIA::TCH szNewFileName[GAIA::MAXPL];
			//	TAST(acc.bindfile(GNIL, __AccType::ACCESS_TYPE_READ | __AccType::ACCESS_TYPE_WRITE, szNewFileName, sizeof(szNewFileName)));
			//	for(GAIA::NUM x = 0; x < 100; ++x)
			//		acc[x] = GAIA::MATH::xrandom();
			//	GAIA::ALGO::bsort(acc, acc + 99);
			//	if(!GAIA::ALGO::issorted(acc, acc + 99))
			//		TERROR;
			//	gdel acc.bindfile();
			//	GAIA::FSYS::Dir dir;
			//	dir.RemoveFile(szNewFileName);
			//}

			/* Accesser write test. */
			{
				GAIA::FSYS::File accfile;
				if(!accfile.Open(strFileName,
					GAIA::FSYS::File::OPEN_TYPE_READ |
					GAIA::FSYS::File::OPEN_TYPE_WRITE |
					GAIA::FSYS::File::OPEN_TYPE_CREATEALWAYS))
				{
					TERROR;
				}
				accfile.Resize(ACCESS_ELEMENT_COUNT);
				typedef GAIA::CTN::Accesser<GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __AccType;
				__AccType acc;
				acc.bindfile(&accfile, __AccType::ACCESS_TYPE_READ | __AccType::ACCESS_TYPE_WRITE);
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT / (GAIA::NUM)sizeof(GAIA::NUM); ++x)
				{
					if(acc.write(&x, sizeof(x)) != sizeof(x))
						TERROR;
				}
			}

			/* Accesser read test. */
			{
				GAIA::FSYS::File accfile;
				if(!accfile.Open(strFileName,
					GAIA::FSYS::File::OPEN_TYPE_READ))
				{
					TERROR;
				}
				accfile.Resize(ACCESS_ELEMENT_COUNT);
				typedef GAIA::CTN::Accesser<GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __AccType;
				__AccType acc;
				acc.bindfile(&accfile, __AccType::ACCESS_TYPE_READ);
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT / (GAIA::NUM)sizeof(GAIA::NUM); ++x)
				{
					GAIA::NUM e;
					if(acc.read(&e, sizeof(e)) != sizeof(e))
					{
						TERROR;
						break;
					}
					if(e != x)
					{
						TERROR;
						break;
					}
				}
			}

			/* Accesser write ovewflow test. */
			{
				GAIA::FSYS::File accfile;
				if(!accfile.Open(strFileName,
					GAIA::FSYS::File::OPEN_TYPE_READ |
					GAIA::FSYS::File::OPEN_TYPE_WRITE |
					GAIA::FSYS::File::OPEN_TYPE_CREATEALWAYS))
				{
					TERROR;
				}
				accfile.Resize(ACCESS_ELEMENT_COUNT);
				typedef GAIA::CTN::Accesser<GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __AccType;
				__AccType acc;
				acc.bindfile(&accfile, __AccType::ACCESS_TYPE_READ | __AccType::ACCESS_TYPE_WRITE);
				GAIA::CTN::Vector<GAIA::NUM> listEle;
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT * 2; ++x)
					listEle.push_back(x);
				if(acc.write(listEle.fptr(), listEle.size() * sizeof(GAIA::NUM)) != ACCESS_ELEMENT_COUNT)
					TERROR;
			}

			/* Accesser read overflow test. */
			{
				GAIA::FSYS::File accfile;
				if(!accfile.Open(strFileName,
					GAIA::FSYS::File::OPEN_TYPE_READ))
				{
					TERROR;
				}
				accfile.Resize(ACCESS_ELEMENT_COUNT);
				typedef GAIA::CTN::Accesser<GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __AccType;
				__AccType acc;
				acc.bindfile(&accfile, __AccType::ACCESS_TYPE_READ);
				GAIA::CTN::Vector<GAIA::NUM> listEle;
				listEle.resize(ACCESS_ELEMENT_COUNT / sizeof(GAIA::NUM) * 2);
				if(acc.read(listEle.fptr(), listEle.size() * sizeof(GAIA::NUM)) != ACCESS_ELEMENT_COUNT)
					TERROR;
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT / (GAIA::NUM)sizeof(GAIA::NUM); ++x)
				{
					if(listEle[x] != x)
					{
						TERROR;
						break;
					}
				}
				GAIA::FSYS::Dir dir;
				dir.RemoveFile(strFileName);
			}

			/* Expandable accesser read write test. */
			{
				typedef GAIA::CTN::Accesser<GAIA::NUM, GAIA::NUM, GAIA::ALGO::ExtendGold<GAIA::NUM> > __AccType;
				__AccType acc;
				GAIA::CH szNewFileName[GAIA::MAXPL];
				TAST(acc.bindfile(GNIL, __AccType::ACCESS_TYPE_READ | __AccType::ACCESS_TYPE_WRITE, szNewFileName, sizeof(szNewFileName)));
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT; ++x)
				{
					if(acc.write(&x, sizeof(x)) != sizeof(x))
					{
						TERROR;
						break;
					}
				}
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT / 10; ++x)
				{
					acc.index(x * 10 + 9);
					GAIA::NUM t = GINVALID;
					if(acc.write(&t, sizeof(t)) != sizeof(t))
					{
						TERROR;
						break;
					}
				}
				acc.index(0);
				for(GAIA::NUM x = 0; x < ACCESS_ELEMENT_COUNT; ++x)
				{
					GAIA::NUM t;
					if(acc.read(&t, sizeof(t)) != sizeof(t))
					{
						TERROR;
						break;
					}
					if((x + 1) % 10 == 0)
					{
						if(t != GINVALID)
						{
							TERROR;
							break;
						}
					}
					else
					{
						if(t != x)
						{
							TERROR;
							break;
						}
					}
				}
				gdel acc.bindfile();
				GAIA::FSYS::Dir dir;
				dir.RemoveFile(szNewFileName);
			}
		}
	}
}
