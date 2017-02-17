#ifndef		__GAIA_MISC_CMDLINE_H__
#define		__GAIA_MISC_CMDLINE_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_string.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_queue.h"
#include "gaia_ctn_vari.h"

namespace GAIA
{
	namespace MISC
	{
		enum CMDLINE_PARAM_TYPE
		{
			CMDLINE_PARAM_AUTO = 0,
			CMDLINE_PARAM_INT64,
			CMDLINE_PARAM_F64,
			CMDLINE_PARAM_STRING,
			CMDLINE_PARAM_BOOL,
			CMDLINE_PARAM_ENUM,
			CMDLINE_PARAM_UUID,
		};

		static const GAIA::CH* CMDLINE_PARAM_TYPE_STRING[] =
		{
			"Auto",
			"Integer",
			"Real",
			"String",
			"Boolean",
			"Enum",
			"UUID",
		};

		class CmdLine : public GAIA::Base
		{
		public:
			typedef GAIA::CH __CharType;
			typedef GAIA::CTN::AString __StringType;

		public:
			CmdLine(){this->init();}
			~CmdLine(){}

			GINL GAIA::BL decl(const __CharType* pszCmd, const __CharType* pszDesc = GNIL)
			{
				GAST(!GAIA::ALGO::gstremp(pszCmd));
				if(GAIA::ALGO::gstremp(pszCmd))
					return GAIA::False;

				Decl d;
				d.strCmd = pszCmd;
				d.strDesc = pszDesc;

				m_decls.push_back(d);
				m_sDeclCursor = m_decls.size() - 1;
				m_sParamCursor = GINVALID;

				return GAIA::True;
			}
			GINL GAIA::BL decl_param(const __CharType* pszParam, const __CharType* pszDesc = GNIL, CMDLINE_PARAM_TYPE paramtype = CMDLINE_PARAM_AUTO, GAIA::BL bMustExist = GAIA::True)
			{
				GAST(!GAIA::ALGO::gstremp(pszParam));
				if(GAIA::ALGO::gstremp(pszParam))
					return GAIA::False;

				if(m_sDeclCursor == GINVALID)
					return GAIA::False;

				Decl& d = m_decls[m_sDeclCursor];

				for(GAIA::NUM x = 0; x < d.params.size(); ++x)
				{
					const DeclParam& temp = d.params[x];
					if(temp.strParam.equals(pszParam))
						return GAIA::False;
				}

				DeclParam param;
				param.strParam = pszParam;
				param.strDesc = pszDesc;
				param.paramtype = paramtype;
				param.bMustExist = bMustExist;

				d.params.push_back(param);
				m_sParamCursor = d.params.size() - 1;

				return GAIA::True;
			}
			GINL GAIA::BL decl_param_range(GAIA::N64 nMin, GAIA::N64 nMax)
			{
				if(m_sDeclCursor == GINVALID)
					return GAIA::False;
				if(m_sParamCursor == GINVALID)
					return GAIA::False;

				Decl& d = m_decls[m_sDeclCursor];
				DeclParam& param = d.params[m_sParamCursor];

				if(nMin > nMax)
					param.bLimitMin = param.bLimitMax = GAIA::False;
				else
				{
					param.bLimitMin = param.bLimitMax = GAIA::True;
					param.limitMin = nMin;
					param.limitMax = nMax;
				}

				return GAIA::True;
			}
			GINL GAIA::BL decl_param_range_min(GAIA::N64 nMin)
			{
				if(m_sDeclCursor == GINVALID)
					return GAIA::False;
				if(m_sParamCursor == GINVALID)
					return GAIA::False;

				Decl& d = m_decls[m_sDeclCursor];
				DeclParam& param = d.params[m_sParamCursor];

				param.bLimitMin = GAIA::True;
				param.limitMin = nMin;

				return GAIA::True;
			}
			GINL GAIA::BL decl_param_range_max(GAIA::N64 nMax)
			{
				if(m_sDeclCursor == GINVALID)
					return GAIA::False;
				if(m_sParamCursor == GINVALID)
					return GAIA::False;

				Decl& d = m_decls[m_sDeclCursor];
				DeclParam& param = d.params[m_sParamCursor];

				param.bLimitMax = GAIA::True;
				param.limitMax = nMax;

				return GAIA::True;
			}
			GINL GAIA::BL decl_param_range(GAIA::F64 fMin, GAIA::F64 fMax)
			{
				if(m_sDeclCursor == GINVALID)
					return GAIA::False;
				if(m_sParamCursor == GINVALID)
					return GAIA::False;

				Decl& d = m_decls[m_sDeclCursor];
				DeclParam& param = d.params[m_sParamCursor];

				if(fMin > fMax)
					param.bLimitMin = param.bLimitMax = GAIA::False;
				else
				{
					param.bLimitMin = param.bLimitMax = GAIA::True;
					param.limitMin = fMin;
					param.limitMax = fMax;
				}

				return GAIA::True;
			}
			GINL GAIA::BL decl_param_range_min(GAIA::F64 fMin)
			{
				if(m_sDeclCursor == GINVALID)
					return GAIA::False;
				if(m_sParamCursor == GINVALID)
					return GAIA::False;

				Decl& d = m_decls[m_sDeclCursor];
				DeclParam& param = d.params[m_sParamCursor];

				param.bLimitMin = GAIA::True;
				param.limitMin = fMin;

				return GAIA::True;
			}
			GINL GAIA::BL decl_param_range_max(GAIA::F64 fMax)
			{
				if(m_sDeclCursor == GINVALID)
					return GAIA::False;
				if(m_sParamCursor == GINVALID)
					return GAIA::False;

				Decl& d = m_decls[m_sDeclCursor];
				DeclParam& param = d.params[m_sParamCursor];

				param.bLimitMax = GAIA::True;
				param.limitMax = fMax;

				return GAIA::True;
			}
			GINL GAIA::BL decl_param_enum(const __CharType* pszEnum)
			{
				GAST(!GAIA::ALGO::gstremp(pszEnum));
				if(GAIA::ALGO::gstremp(pszEnum))
					return GAIA::False;

				if(m_sDeclCursor == GINVALID)
					return GAIA::False;
				if(m_sParamCursor == GINVALID)
					return GAIA::False;

				Decl& d = m_decls[m_sDeclCursor];
				DeclParam& param = d.params[m_sParamCursor];

				if(param.limitEnums.find(pszEnum, 0) != GINVALID)
					return GAIA::False;

				param.limitEnums.push_back(pszEnum);
				return GAIA::True;
			}

			GINL GAIA::BL exec(GAIA::STREAM::StreamBase& stm)
			{
				if(this->exec_step(stm))
					return GAIA::True;

				__CharType szTemp[1024];
				for(;;)
				{
					stm >> szTemp;
					if(szTemp[0] == '\0')
						continue;
					m_remain += " ";
					m_remain += szTemp;
					if(GAIA::ALGO::gstrch(szTemp, ';') != GNIL)
					{
						if(this->exec_step(stm))
							break;
					}
				}
				return GAIA::True;
			}
			GINL __StringType pop()
			{
				if(m_res.empty())
					return "";
				__StringType ret = m_res.front();
				m_res.pop_front();
				return ret;
			}
			GINL GAIA::NUM remain()
			{
				return m_res.size();
			}
			GINL __StringType tostring(const __CharType* pszCmd = GNIL, const __CharType* pszParam = GNIL) const
			{
				static const GAIA::NUM CMD_ALIGN = 16;

				__StringType strRet;
				if(!GAIA::ALGO::gstremp(pszCmd))
				{
					for(GAIA::NUM x = 0; x < m_decls.size(); ++x)
					{
						const Decl& d = m_decls[x];
						if(!d.strCmd.equals(pszCmd))
							continue;

						if(GAIA::ALGO::gstremp(pszParam))
						{
							__StringType strTemp;
							strTemp = d.strCmd;
							if(!d.params.empty())
							{
								strTemp += "(";
								strTemp += d.params.size();
								strTemp += ")";
							}
							strRet += strTemp;
							for(GAIA::NUM y = CMD_ALIGN - strTemp.size(); y >= 0; --y)
								strRet += " ";
							strRet += d.strDesc;
							strRet += "\n";
							for(GAIA::NUM y = 0; y < d.params.size(); ++y)
							{
								const DeclParam& dp = d.params[y];
								strTemp = "-";
								strTemp += dp.strParam;
								strRet += strTemp;
								for(GAIA::NUM z = CMD_ALIGN - strTemp.size(); z >= 0; --z)
									strRet += " ";
								strRet += dp.strDesc;
								strRet += "\n";
							}
							strRet += "\n\n";
						}
						else
						{
							for(GAIA::NUM y = 0; y < d.params.size(); ++y)
							{
								const DeclParam& dp = d.params[y];
								if(dp.strParam.equals(pszParam))
								{
									strRet += dp.strParam;
									strRet += " ";
									strRet += "Type=";
									strRet += CMDLINE_PARAM_TYPE_STRING[dp.paramtype];
									strRet += " ";
									if(!dp.bMustExist)
										strRet += "NotMustExist";
									strRet += "\n";
									if(dp.bLimitMin)
									{
										strRet += "LimitMin=";
										if(dp.limitMin.type() == GAIA::TYPEID_N64)
											strRet += (GAIA::N64)dp.limitMin;
										else if(dp.limitMin.type() == GAIA::TYPEID_F64)
											strRet += (GAIA::F64)dp.limitMin;
										strRet += "\n";
									}
									if(dp.bLimitMax)
									{
										strRet += "LimitMax=";
										if(dp.limitMax.type() == GAIA::TYPEID_N64)
											strRet += (GAIA::N64)dp.limitMax;
										else if(dp.limitMax.type() == GAIA::TYPEID_F64)
											strRet += (GAIA::F64)dp.limitMax;
										strRet += "\n";
									}
									if(!dp.limitEnums.empty())
									{
										strRet += "Enums=";
										for(GAIA::NUM z = 0; z < dp.limitEnums.size(); ++z)
										{
											if(z != 0)
												strRet += ", ";
											strRet += dp.limitEnums[z];
										}
									}
									strRet += "\n";
									return strRet;
								}
							}
						}
					}
				}
				else
				{
					for(GAIA::NUM x = 0; x < m_decls.size(); ++x)
					{
						const Decl& d = m_decls[x];
						__StringType strTemp;
						strTemp += d.strCmd;
						if(!d.params.empty())
						{
							strTemp += "(";
							strTemp += d.params.size();
							strTemp += ")";
						}
						strRet += strTemp;
						for(GAIA::NUM y = CMD_ALIGN - strTemp.size(); y >= 0; --y)
							strRet += " ";
						strRet += d.strDesc;
						strRet += "\n";
					}
				}
				return strRet;
			}

		private:
			class DeclParam : public GAIA::Base
			{
			public:
				GINL DeclParam()
				{
					paramtype = CMDLINE_PARAM_AUTO;
					bMustExist = GAIA::True;
					bLimitMin = GAIA::False;
					bLimitMax = GAIA::False;
					limitMin = 0;
					limitMax = 0;
				}
			public:
				__StringType strParam;
				__StringType strDesc;
				CMDLINE_PARAM_TYPE paramtype;
				GAIA::BL bMustExist;
				GAIA::BL bLimitMin;
				GAIA::BL bLimitMax;
				GAIA::CTN::Vari limitMin;
				GAIA::CTN::Vari limitMax;
				GAIA::CTN::Vector<__StringType> limitEnums;
			};
			class Decl : public GAIA::Base
			{
			public:
				GINL Decl()
				{
				}
			public:
				__StringType strCmd;
				__StringType strDesc;
				GAIA::CTN::Vector<DeclParam> params;
			};

		private:
			GINL GAIA::GVOID init()
			{
				m_sDeclCursor = GINVALID;
				m_sParamCursor = GINVALID;
			}

			GINL GAIA::BL exec_step(GAIA::STREAM::StreamBase& stm)
			{
				for(;;)
				{
					// Split remain.
					GAIA::NUM sRemainSplit = m_remain.find(';', 0);
					if(sRemainSplit != GINVALID)
					{
						__StringType strCmd = m_remain;
						strCmd.left(sRemainSplit);
						m_remain.right(sRemainSplit);

						// Split command.
						GAIA::CTN::Vector<__StringType> sections;
						for(; !strCmd.empty();)
						{
							strCmd.trims(' ');
							GAIA::NUM sCmdSplit = strCmd.find(' ', 0);
							if(sCmdSplit == GINVALID)
							{
								sections.push_back(strCmd);
								strCmd.clear();
							}
							else
							{
								__StringType strPart = strCmd;
								strPart.left(sCmdSplit);
								sections.push_back(strPart);
								strCmd.right(sCmdSplit);
							}
							strCmd.trims(' ');
						}

						// Trims.
						for(GAIA::NUM x = 0; x < sections.size(); ++x)
						{
							__StringType& str = sections[x];
							str.trims(' ');
						}

						// Check.
						if(!this->check(stm, sections))
							continue;

						// Swap to resume.
						for(GAIA::NUM x = 0; x < sections.size(); ++x)
						{
							__StringType& str = sections[x];
							m_res.push_back(str);
						}
						return GAIA::True;
					}
					else
						break;
				}
				return GAIA::False;
			}

			GINL GAIA::BL check(GAIA::STREAM::StreamBase& stm, const GAIA::CTN::Vector<__StringType>& sections) const
			{
				GAIA::BL bExistCmd = GAIA::False;
				GAIA::BL bParamCountNotMatch = GAIA::True;
				for(GAIA::NUM x = 0; x < m_decls.size(); ++x)
				{
					const Decl& d = m_decls[x];
					GAIA::BL bCmdEqual = d.strCmd.equals(sections[0]);
					if(bCmdEqual)
						bExistCmd = GAIA::True;
					if(bCmdEqual && d.params.size() + 1 == sections.size())
					{
						bParamCountNotMatch = GAIA::False;
						GAIA::BL bCheckResult = GAIA::True;
						for(GAIA::NUM y = 0; y < d.params.size(); ++y)
						{
							const DeclParam& dp = d.params[y];
							const __StringType& strParam = sections[y + 1];
							GAIA::U32 paramstrtype = strParam.type();
							do
							{
								if(dp.paramtype != CMDLINE_PARAM_AUTO)
								{
									switch(dp.paramtype)
									{
									case CMDLINE_PARAM_INT64:
										{
											if(!(paramstrtype & GAIA::STRING_TYPE_INTEGER))
											{
												bCheckResult = GAIA::False;
												stm << "Parameter" << y + 1 << " " << strParam.fptr() << " is not a integer!\n";
											}
										}
										break;
									case CMDLINE_PARAM_F64:
										{
											if(!(paramstrtype & GAIA::STRING_TYPE_REAL || paramstrtype & GAIA::STRING_TYPE_INTEGER))
											{
												bCheckResult = GAIA::False;
												stm << "Parameter" << y + 1 << " " << strParam.fptr() << " is not a real or integer!\n";
											}
										}
										break;
									case CMDLINE_PARAM_STRING:
										{
										}
										break;
									case CMDLINE_PARAM_BOOL:
										{
											if(!(strParam != "1" && strParam != "0"))
											{
												bCheckResult = GAIA::False;
												stm << "Parameter" << y + 1 << " " << strParam.fptr() << " is not a boolean(0, or 1)!\n";
											}
										}
										break;
									case CMDLINE_PARAM_ENUM:
										{
										}
										break;
									case CMDLINE_PARAM_UUID:
										{
											if(!(paramstrtype & GAIA::STRING_TYPE_RID128))
											{
												bCheckResult = GAIA::False;
												stm << "Parameter" << y + 1 << " " << strParam.fptr() << " is not a UUID!\n";
											}
										}
										break;
									default:
										break;
									}
									if(!bCheckResult)
										break;
								}

								if(dp.bLimitMin)
								{
									if(dp.limitMin.type() == GAIA::TYPEID_N64)
									{
										GAIA::N64 nLimitMin = dp.limitMin;
										if(paramstrtype & GAIA::STRING_TYPE_INTEGER)
										{
											if((GAIA::N64)strParam < nLimitMin)
											{
												bCheckResult = GAIA::False;
												stm << "Parameter" << y + 1 << " " << strParam.fptr() << " must above equal" << nLimitMin << "!\n";
											}
										}
										else if(paramstrtype & GAIA::STRING_TYPE_REAL)
										{
											if((GAIA::F64)strParam < nLimitMin)
											{
												bCheckResult = GAIA::False;
												stm << "Parameter" << y + 1 << " " << strParam.fptr() << " must above equal" << nLimitMin << "!\n";
											}
										}
										else
										{
											bCheckResult = GAIA::False;
											stm << "Parameter" << y + 1 << " " << strParam.fptr() << " is not a integer or real\n";
										}
									}
									else if(dp.limitMin.type() == GAIA::TYPEID_F64)
									{
										GAIA::F64 fLimitMin = dp.limitMin;
										if(paramstrtype & GAIA::STRING_TYPE_INTEGER)
										{
											if((GAIA::N64)strParam < fLimitMin)
											{
												bCheckResult = GAIA::False;
												stm << "Parameter" << y + 1 << " " << strParam.fptr() << " must above equal" << fLimitMin << "!\n";
											}
										}
										else if(paramstrtype & GAIA::STRING_TYPE_REAL)
										{
											if((GAIA::F64)strParam < fLimitMin)
											{
												bCheckResult = GAIA::False;
												stm << "Parameter" << y + 1 << " " << strParam.fptr() << " must above equal" << fLimitMin << "!\n";
											}
										}
										else
										{
											bCheckResult = GAIA::False;
											stm << "Parameter" << y + 1 << " " << strParam.fptr() << " is not a integer or real\n";
										}
									}
									else
									{
										bCheckResult = GAIA::False;
										stm << "Invalid limit value!\n";
									}
									if(!bCheckResult)
										break;
								}

								if(dp.bLimitMax)
								{
									if(dp.limitMax.type() == GAIA::TYPEID_N64)
									{
										GAIA::N64 nLimitMax = dp.limitMax;
										if(paramstrtype & GAIA::STRING_TYPE_INTEGER)
										{
											if((GAIA::N64)strParam > nLimitMax)
											{
												bCheckResult = GAIA::False;
												stm << "Parameter" << y + 1 << " " << strParam.fptr() << " must below equal" << nLimitMax << "!\n";
											}
										}
										else if(paramstrtype & GAIA::STRING_TYPE_REAL)
										{
											if((GAIA::F64)strParam > nLimitMax)
											{
												bCheckResult = GAIA::False;
												stm << "Parameter" << y + 1 << " " << strParam.fptr() << " must below equal" << nLimitMax << "!\n";
											}
										}
										else
										{
											bCheckResult = GAIA::False;
											stm << "Parameter" << y + 1 << " " << strParam.fptr() << " is not a integer or real\n";
										}
									}
									else if(dp.limitMax.type() == GAIA::TYPEID_F64)
									{
										GAIA::F64 fLimitMax = dp.limitMax;
										if(paramstrtype & GAIA::STRING_TYPE_INTEGER)
										{
											if((GAIA::N64)strParam > fLimitMax)
											{
												bCheckResult = GAIA::False;
												stm << "Parameter" << y + 1 << " " << strParam.fptr() << " must below equal" << fLimitMax << "!\n";
											}
										}
										else if(paramstrtype & GAIA::STRING_TYPE_REAL)
										{
											if((GAIA::F64)strParam > fLimitMax)
											{
												bCheckResult = GAIA::False;
												stm << "Parameter" << y + 1 << " " << strParam.fptr() << " must below equal" << fLimitMax << "!\n";
											}
										}
										else
										{
											bCheckResult = GAIA::False;
											stm << "Parameter" << y + 1 << " " << strParam.fptr() << " is not a integer or real\n";
										}
									}
									else
									{
										bCheckResult = GAIA::False;
										stm << "Invalid limit value!\n";
									}
									if(!bCheckResult)
										break;
								}

								if(!dp.limitEnums.empty())
								{
									GAIA::BL bMatch = GAIA::False;
									for(GAIA::NUM z = 0; z < dp.limitEnums.size(); ++z)
									{
										if(dp.limitEnums[x].equals(strParam))
										{
											bMatch = GAIA::True;
											break;
										}
									}
									if(!bMatch)
									{
										bCheckResult = GAIA::False;
										stm << "Parameter" << y + 1 << " " << strParam.fptr() << " is not a valid enum!\n";
										break;
									}
								}
							}
							while(GAIA::ALWAYSFALSE);

							if(!bCheckResult)
								break;
						}
						if(bCheckResult)
							return GAIA::True;
					}
				}

				if(!bExistCmd)
					stm << "Invalid command!\n";
				else if(bParamCountNotMatch)
					stm << "Command parameter count error!\n";

				return GAIA::False;
			}

		private:
			GAIA::CTN::Vector<Decl> m_decls;
			__StringType m_remain;
			GAIA::CTN::Queue<__StringType> m_res;
			GAIA::NUM m_sDeclCursor;
			GAIA::NUM m_sParamCursor;
		};
	}
}

#endif
