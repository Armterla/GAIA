#ifndef 	__GAIA_MISC_CMDPARAM_H__
#define 	__GAIA_MISC_CMDPARAM_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_ctn_vector.h"
#include "gaia_ctn_string.h"

namespace GAIA
{
	namespace MISC
	{
		static const GAIA::TCH* CMD_TYPE_NAME[] =
		{
			_T("Invalid"),
			_T("MustExist"),
		};

		static const GAIA::TCH* PARAM_TYPE_NAME[] =
		{
			_T("Invalid"),
			_T("File"),
			_T("MustExistFile"),
			_T("MustNotExistFile"),
			_T("Int"),
			_T("String"),
			_T("Enum"),
		};

		class CmdParam : public GAIA::Base
		{
		public:
			GINL CmdParam(){this->init();}

		public: /* For format declaration. */
			GAIA_ENUM_BEGIN(CMD_TYPE)
				CMD_TYPE_MUSTEXIST,
			GAIA_ENUM_END(CMD_TYPE)

			GAIA_ENUM_BEGIN(PARAM_TYPE)
				PARAM_TYPE_FILE,
				PARAM_TYPE_MUSTEXISTFILE,
				PARAM_TYPE_MUSTNOTEXISTFILE,
				PARAM_TYPE_INT,
				PARAM_TYPE_STRING,
				PARAM_TYPE_ENUM,
			GAIA_ENUM_END(PARAM_TYPE)

			GINL GAIA::BL begin_decl()
			{
				if(this->is_begin_decl())
					return GAIA::False;
				m_bBeginDecl = GAIA::True;
				return GAIA::True;
			}
			GINL GAIA::BL end_decl()
			{
				if(!this->is_begin_decl())
					return GAIA::False;
				m_opt_decls.clear();
				for(__CmdDeclListType::_sizetype x = 0; x < m_decls.size(); ++x)
					m_opt_decls.push_back(&m_decls[x]);
				m_opt_decls.sort();
				m_opt_cmds.clear();
				for(__CmdListType::_sizetype x = 0; x < m_cmds.size(); ++x)
					m_opt_cmds.push_back(&m_cmds[x]);
				m_opt_cmds.sort();
				m_bBeginDecl = GAIA::False;
				return GAIA::True;
			}
			GINL GAIA::BL is_begin_decl() const{return m_bBeginDecl;}
			GINL GAIA::NUM cmd_decl(
				const GAIA::TCH* pszCmd,
				const GAIA::TCH* pszDesc,
				const GAIA::NUM& min_param_size,
				const GAIA::NUM& max_param_size,
				CMD_TYPE t)
			{
				GAST(!GAIA::ALGO::gstremp(pszCmd));
				if(GAIA::ALGO::gstremp(pszCmd))
					return GINVALID;
				GAST(!GAIA::ALGO::gstremp(pszDesc));
				if(GAIA::ALGO::gstremp(pszDesc))
					return GINVALID;
				GAST(min_param_size >= 0);
				if(min_param_size < 0)
					return GINVALID;
				GAST(max_param_size >= 0 || max_param_size == GINVALID);
				if(max_param_size < 0 && max_param_size != GINVALID)
					return GINVALID;
				GAST(min_param_size <= max_param_size || max_param_size == GINVALID);
				if(min_param_size > max_param_size && max_param_size != GINVALID)
					return GINVALID;
				GAST(this->is_begin_decl());
				if(!this->is_begin_decl())
					return GINVALID;
				CmdDecl temp;
				temp.cmd = pszCmd;
				temp.desc = pszDesc;
				temp.type = t;
				temp.min_param_size = min_param_size;
				temp.max_param_size = max_param_size;
				for(__CmdDeclListType::_sizetype x = 0; x < m_decls.size(); ++x)
				{
					if(m_decls[x] == temp)
						return GINVALID;
				}
				temp.index = m_decls.size();
				m_decls.push_back(temp);
				return m_decls.size() - 1;
			}
			GINL GAIA::BL cmd_param_decl(
				const GAIA::TCH* pszCmd,
				const GAIA::NUM& min_param_size,
				const GAIA::NUM& max_param_size,
				const GAIA::NUM& start_param_index,
				const GAIA::NUM& end_param_index,
				const GAIA::TCH* pszDesc,
				PARAM_TYPE t)
			{
				GAST(!GAIA::ALGO::gstremp(pszCmd));
				if(GAIA::ALGO::gstremp(pszCmd))
					return GAIA::False;
				GAST(min_param_size >= 0);
				if(min_param_size < 0)
					return GAIA::False;
				GAST(max_param_size >= 0 || max_param_size == GINVALID);
				if(max_param_size < 0 && max_param_size != GINVALID)
					return GAIA::False;
				GAST(min_param_size <= max_param_size || max_param_size == GINVALID);
				if(min_param_size > max_param_size && max_param_size != GINVALID)
					return GAIA::False;
				GAST(start_param_index >= 0);
				if(start_param_index < 0)
					return GAIA::False;
				GAST(end_param_index >= 0 || end_param_index == GINVALID);
				if(end_param_index < 0 && end_param_index != GINVALID)
					return GAIA::False;
				GAST(start_param_index <= end_param_index || end_param_index == GINVALID);
				if(start_param_index > end_param_index && end_param_index != GINVALID)
					return GAIA::False;
				GAST(this->is_begin_decl());
				if(!this->is_begin_decl())
					return GAIA::False;
				CmdDecl* pCmd = GNIL;
				for(__CmdDeclListType::_sizetype x = 0; x < m_decls.size(); ++x)
				{
					CmdDecl& temp = m_decls[x];
					if(temp.cmd == pszCmd &&
						temp.min_param_size == min_param_size &&
						temp.max_param_size == max_param_size)
					{
						pCmd = &temp;
						break;
					}
				}
				if(pCmd == GNIL)
					return GAIA::False;
				ParamDecl temp;
				temp.start_param_index = start_param_index;
				temp.end_param_index = end_param_index;
				if(!GAIA::ALGO::gstremp(pszDesc))
					temp.desc = pszDesc;
				temp.type = t;
				for(__ParamDeclListType::_sizetype x = 0; x < pCmd->paramdecls.size(); ++x)
				{
					if(pCmd->paramdecls[x] == temp)
						return GAIA::False;
				}
				pCmd->paramdecls.push_back(temp);
				return GAIA::True;
			}
			GINL GAIA::GVOID cmd_mutex_decl(const GAIA::TCH* pszCmd1, const GAIA::TCH* pszCmd2)
			{
				GAST(!GAIA::ALGO::gstremp(pszCmd1));
				if(GAIA::ALGO::gstremp(pszCmd1))
					return;
				GAST(!GAIA::ALGO::gstremp(pszCmd2));
				if(GAIA::ALGO::gstremp(pszCmd2))
					return;
				GAST(this->is_begin_decl());
				if(!this->is_begin_decl())
					return;
				if(GAIA::ALGO::gstrcmp(pszCmd1, pszCmd2) > 0)
					GAIA::ALGO::swap(pszCmd1, pszCmd2);
				for(__CmdMutexListType::_sizetype x = 0; x < m_mutex_decls.size(); ++x)
				{
					CmdMutex& temp = m_mutex_decls[x];
					if(temp.cmd1 == pszCmd1 &&
						temp.cmd2 == pszCmd2)
						return;
				}
				CmdMutex temp;
				m_mutex_decls.push_back(temp);
				m_mutex_decls.back().cmd1 = pszCmd1;
				m_mutex_decls.back().cmd2 = pszCmd2;
			}
			GINL GAIA::GVOID cmd_coop_decl(const GAIA::TCH* pszCmd1, const GAIA::TCH* pszCmd2)
			{
				GAST(!GAIA::ALGO::gstremp(pszCmd1));
				if(GAIA::ALGO::gstremp(pszCmd1))
					return;
				GAST(!GAIA::ALGO::gstremp(pszCmd2));
				if(GAIA::ALGO::gstremp(pszCmd2))
					return;
				GAST(this->is_begin_decl());
				if(!this->is_begin_decl())
					return;
				if(GAIA::ALGO::gstrcmp(pszCmd1, pszCmd2) > 0)
					GAIA::ALGO::swap(pszCmd1, pszCmd2);
				for(__CmdCoopListType::_sizetype x = 0; x < m_coop_decls.size(); ++x)
				{
					CmdCoop& temp = m_coop_decls[x];
					if(temp.cmd1 == pszCmd1 &&
						temp.cmd2 == pszCmd2)
						return;
				}
				CmdCoop temp;
				m_coop_decls.push_back(temp);
				m_coop_decls.back().cmd1 = pszCmd1;
				m_coop_decls.back().cmd2 = pszCmd2;
			}
			GINL GAIA::NUM cmd_decl_size() const{return m_decls.size();}
			GINL const GAIA::TCH* cmd_decl_cmd(const GAIA::NUM& decl) const
			{
				GAST(decl < m_decls.size());
				if(decl >= m_decls.size())
					return GNIL;
				return m_decls[decl].cmd;
			}
			GINL const GAIA::TCH* cmd_decl_desc(const GAIA::NUM& decl) const
			{
				GAST(decl < m_decls.size());
				if(decl >= m_decls.size())
					return GNIL;
				return m_decls[decl].desc;
			}
			GINL GAIA::NUM cmd_decl_min_param_size(const GAIA::NUM& decl) const
			{
				GAST(decl < m_decls.size());
				if(decl >= m_decls.size())
					return (GAIA::NUM)GINVALID;
				return m_decls[decl].min_param_size;
			}
			GINL GAIA::NUM cmd_decl_max_param_size(const GAIA::NUM& decl) const
			{
				GAST(decl < m_decls.size());
				if(decl >= m_decls.size())
					return (GAIA::NUM)GINVALID;
				return m_decls[decl].max_param_size;
			}
			GINL CMD_TYPE cmd_decl_type(const GAIA::NUM& decl) const
			{
				GAST(decl < m_decls.size());
				if(decl >= m_decls.size())
					return CMD_TYPE_INVALID;
				return m_decls[decl].type;
			}
			GINL GAIA::GVOID clear_decl()
			{
				m_decls.clear();
				m_opt_decls.clear();
				m_mutex_decls.clear();
				m_coop_decls.clear();
			}
			GINL GAIA::NUM finddecl(const GAIA::TCH* pszCmdName, const GAIA::NUM& min_param_size) const
			{
				GAST(!GAIA::ALGO::gstremp(pszCmdName));
				if(GAIA::ALGO::gstremp(pszCmdName))
					return GINVALID;
				GAST(!this->is_begin_decl());
				if(this->is_begin_decl())
					return GINVALID;
				CmdDecl cmddecl;
				cmddecl.cmd = pszCmdName;
				cmddecl.min_param_size = min_param_size;
				__CmdDeclOPTListType::_datatype t = &cmddecl;
				__CmdDeclOPTListType::const_it it = m_opt_decls.lower_equal(t);
				if(it.empty())
					return GINVALID;
				if((**it).cmd != pszCmdName)
					return GINVALID;
				return (**it).index;
			}

			GINL GAIA::CTN::TString tostring()
			{
				GAIA::CTN::TString str;
				for(GAIA::NUM x = 0; x < m_decls.size(); ++x)
				{
					const CmdDecl& decl = m_decls[x];
					GAIA::CTN::TString strTemp;
					strTemp += decl.cmd;
					if(decl.min_param_size != 0 && decl.max_param_size != 0)
					{
						if(decl.min_param_size == decl.max_param_size)
						{
							GAIA::TCH szTemp[32];
							GAIA::ALGO::castv(decl.min_param_size, szTemp, sizeofarray(szTemp));
							strTemp += (GAIA::CTN::TString)_T("(") + (GAIA::CTN::TString)szTemp + (GAIA::CTN::TString)_T(")");
						}
						else
						{
							GAIA::TCH szTemp1[32];
							GAIA::TCH szTemp2[32];
							GAIA::ALGO::castv(decl.min_param_size, szTemp1, sizeofarray(szTemp1));
							GAIA::ALGO::castv(decl.max_param_size, szTemp2, sizeofarray(szTemp2));
							strTemp += (GAIA::CTN::TString)_T("(") + (GAIA::CTN::TString)szTemp1 + (GAIA::CTN::TString)_T(", ") + (GAIA::CTN::TString)szTemp2 + (GAIA::CTN::TString)_T(")");
						}
					}
					str += "    ";
					str += strTemp;
					for(GAIA::NUM y = strTemp.size(); y < 24; ++y)
						str += " ";
					str += decl.desc;
					str += "\n";
					for(GAIA::NUM y = 0; decl.paramdecls.size(); ++y)
					{
						const ParamDecl& pd = decl.paramdecls[y];
						str += "[";
						str += pd.start_param_index;
						str += " ,";
						str += pd.end_param_index;
						str += "] ";
						str += GAIA::MISC::PARAM_TYPE_NAME[pd.type];
						str += " ";
						str += pd.desc;
						str += "\n";
					}
				}
				return str;
			}

		public:	/* Command access interface. */
			GINL GAIA::BL build(const GAIA::TCH* pszCmd, GAIA::STREAM::StreamBase& stm)
			{
				GAST(!this->is_begin_decl());
				if(this->is_begin_decl())
					return GAIA::False;
				GAST(!GAIA::ALGO::gstremp(pszCmd));
				if(GAIA::ALGO::gstremp(pszCmd))
					return GAIA::False;

				GAIA::BL bCheckSuccess = GAIA::False;

				/* Construct word list. */
				__ParamListType words;
				const GAIA::TCH* p = pszCmd;
				const GAIA::TCH* pLast = p;
				__StringType str;
				while(*p != 0 || pLast != p)
				{
					if(*p == '\r' || *p == '\n' || *p == ' ' || *p == '\t' || *p == '\0')
					{
						if(p > pLast)
						{
							str.resize((__StringType::_sizetype)(p - pLast));
							GAIA::ALGO::gmemcpy(str.fptr(), pLast, (p - pLast) * sizeof(__StringType::_datatype));
							__StringType::_sizetype index_begin = 0, index_end = str.size() - 1;
							for(; index_begin < str.size(); ++index_begin)
							{
								if(str[index_begin] != '\r' &&
									str[index_begin] != '\n' &&
									str[index_begin] != ' ' &&
									str[index_begin] != '\t')
									break;
							}
							for(;;)
							{
								if(str[index_end] != '\r' &&
									str[index_end] != '\n' &&
									str[index_end] != ' ' &&
									str[index_end] != '\t')
									break;
								--index_end;
								if(index_end == GINVALID)
									break;
							}
							if(index_begin != str.size() && index_end != GINVALID)
							{
								str.mid(index_begin, index_end);
								words.push_back(str);
							}
							if(*p == 0)
							{
								pLast = p;
								break;
							}
							else
								pLast = p + 1;
						}
					}
					++p;
				}

				/* Analyze to command. */
				for(__ParamListType::_sizetype x = 0; x < words.size(); ++x)
				{
					GAIA::BL bMatch = GAIA::False;
					__StringType& word = words[x];
					if(m_decls.empty())
						bMatch = GAIA::True;
					else
					{
						if(this->finddecl(word, GAIA::MAXSIZE) != GINVALID)
							bMatch = GAIA::True;
					}
					if(bMatch)
					{
						Cmd cmd;
						cmd.index = m_cmds.size();
						cmd.cmd = word;
						m_cmds.push_back(cmd);
					}
					else
					{
						if(m_cmds.empty())
						{
							stm << "CmdParam:Invalid command or parameter without command! \"" << word.fptr() << "\"\n";
							goto FUNCTION_END;
						}
						m_cmds.back().params.push_back(word);
					}
				}

				/* Checkup. */
				for(__CmdListType::_sizetype x = 0; x < m_cmds.size(); ++x)
				{
					const Cmd& temp = m_cmds[x];
					GAIA::NUM uDecl = this->finddecl(temp.cmd, temp.params.size());
					if(uDecl == GINVALID)
					{
						stm << "CmdParam:Invalid command! \"" << temp.cmd.fptr() << "\"\n";
						goto FUNCTION_END;
					}
					CmdDecl& decl = m_decls[uDecl];
					if(temp.params.size() < decl.min_param_size)
					{
						stm << "CmdParam:Insufficient parameter count! \"" << temp.cmd.fptr() << "\"" <<
							" [" << decl.min_param_size << ", " << decl.max_param_size << "]" << "\n";
						goto FUNCTION_END;
					}
					if(decl.max_param_size != GINVALID)
					{
						if(temp.params.size() > decl.max_param_size)
						{
							stm << "CmdParam:Insufficient parameter count! \"" << temp.cmd.fptr() << "\"" <<
								" [" << decl.min_param_size << ", " << decl.max_param_size << "]" << "\n";
							goto FUNCTION_END;
						}
					}
				}
				for(GAIA::NUM x = 0; x < m_decls.size(); ++x)
				{
					CmdDecl& decl = m_decls[x];
					if(decl.type == CMD_TYPE_MUSTEXIST)
					{
						GAIA::BL bExist = GAIA::False;
						for(GAIA::NUM y = 0; y < m_cmds.size(); ++y)
						{
							const Cmd& temp = m_cmds[x];
							if(temp.cmd == decl.cmd)
							{
								bExist = GAIA::True;
								break;
							}
						}
						if(!bExist)
						{
							stm << "CmdParam:Must exist command! \"" << decl.cmd.fptr() << "\"\n";
							goto FUNCTION_END;
						}
					}
				}

				/* Checkup mutex and coop. */
				for(__CmdListType::_sizetype x = 0; x < m_cmds.size(); ++x)
				{
					Cmd& temp = m_cmds[x];
					for(__CmdMutexListType::_sizetype y = 0; y < m_mutex_decls.size(); ++y)
					{
						const CmdMutex& cm = m_mutex_decls[y];
						const __StringType* pCmd = GNIL;
						if(cm.cmd1 == temp.cmd)
							pCmd = &cm.cmd2;
						else if(cm.cmd2 == temp.cmd)
							pCmd = &cm.cmd1;
						if(pCmd != GNIL)
						{
							for(__CmdListType::_sizetype z = 0; z < m_cmds.size(); ++z)
							{
								if(z == x)
									continue;
								if(m_cmds[z].cmd == *pCmd)
								{
									stm << "CmdParam:Command mutex! \"" << temp.cmd.fptr() << "\" " << "and" << "\"" << pCmd->fptr() << "\"\n";
									goto FUNCTION_END;
								}
							}
						}
					}
					for(__CmdCoopListType::_sizetype y = 0; y < m_coop_decls.size(); ++y)
					{
						const CmdCoop& cc = m_coop_decls[y];
						const __StringType* pCmd = GNIL;
						if(cc.cmd1 == temp.cmd)
							pCmd = &cc.cmd2;
						else if(cc.cmd2 == temp.cmd)
							pCmd = &cc.cmd1;
						if(pCmd != GNIL)
						{
							GAIA::BL bExist = GAIA::False;
							for(__CmdListType::_sizetype z = 0; z < m_cmds.size(); ++z)
							{
								if(z == x)
									continue;
								if(m_cmds[z].cmd == *pCmd)
								{
									bExist = GAIA::True;
									break;
								}
							}
							if(!bExist)
							{
								stm << "CmdParam:Command coop! \"" << temp.cmd.fptr() << "\" " << "and" << "\"" << pCmd->fptr() << "\"\n";
								goto FUNCTION_END;
							}
						}
					}
				}

				/* Optimize. */
				m_opt_cmds.clear();
				for(__CmdListType::_sizetype x = 0; x < m_cmds.size(); ++x)
				{
					__CmdOPTListType::_datatype t;
					t = &m_cmds[x];
					m_opt_cmds.push_back(t);
				}
				m_opt_cmds.sort();

				/* Sign to success. */
				bCheckSuccess = GAIA::True;

			FUNCTION_END:
				if(!bCheckSuccess)
				{
					this->clear_cmd();
					return GAIA::False;
				}
				return GAIA::True;
			}
			GINL GAIA::GVOID clear_cmd()
			{
				m_cmds.clear();
				m_opt_cmds.clear();
			}
			GINL GAIA::NUM cmd_size() const
			{
				return m_cmds.size();
			}
			const GAIA::TCH* cmd(const GAIA::NUM& cmd) const
			{
				if(cmd >= this->cmd_size())
					return GNIL;
				return m_cmds[cmd].cmd.fptr();
			}
			GINL GAIA::NUM param_size(const GAIA::NUM& cmd) const
			{
				if(cmd >= this->cmd_size())
					return GNIL;
				return m_cmds[cmd].params.size();
			}
			const GAIA::TCH* param(const GAIA::NUM& cmd, const GAIA::NUM& param) const
			{
				if(cmd >= this->cmd_size())
					return GNIL;
				if(param >= this->param_size(cmd))
					return GNIL;
				return m_cmds[cmd].params[param];
			}
			GINL GAIA::NUM findcmd(const GAIA::TCH* pszCmd) const
			{
				GAST(!GAIA::ALGO::gstremp(pszCmd));
				if(GAIA::ALGO::gstremp(pszCmd))
					return GINVALID;
				GAST(!this->is_begin_decl());
				if(this->is_begin_decl())
					return GINVALID;
				__CmdOPTListType::_datatype t;
				Cmd cmd;
				cmd.cmd = pszCmd;
				t = &cmd;
				__CmdOPTListType::const_it it = m_opt_cmds.upper_equal(t);
				if(it.empty())
					return GINVALID;
				if((**it).cmd != pszCmd)
					return GINVALID;
				return (**it).index;
			}

			GINL CmdParam& operator = (const CmdParam& src)
			{
				GAST(!src.is_begin_decl());
				GAST(!this->is_begin_decl());
				m_bBeginDecl = GAIA::False;
				m_decls = src.m_decls;
				m_opt_decls.clear();
				for(GAIA::NUM x = 0; x < m_decls.size(); ++x)
					m_opt_decls.push_back(&m_decls[x]);
				m_opt_decls.sort();
				m_mutex_decls = src.m_mutex_decls;
				m_coop_decls = src.m_coop_decls;
				m_cmds = src.m_cmds;
				m_opt_cmds.clear();
				for(GAIA::NUM x = 0; x < m_cmds.size(); ++x)
					m_opt_cmds.push_back(&m_cmds[x]);
				m_opt_cmds.sort();
				return *this;
			}
			GCLASS_COMPARE3(m_opt_decls, m_mutex_decls, m_coop_decls, CmdParam)

		private:
			typedef GAIA::CTN::TString __StringType;
			class ParamDecl
			{
			public:
				GINL ParamDecl& operator = (const ParamDecl& src)
				{
					start_param_index = src.start_param_index;
					end_param_index = src.end_param_index;
					desc = src.desc;
					type = src.type;
					return *this;
				}
				GCLASS_COMPARE2(start_param_index, end_param_index, ParamDecl)
			public:
				GAIA::NUM start_param_index;
				GAIA::NUM end_param_index;
				__StringType desc;
				PARAM_TYPE type;
			};
			typedef GAIA::CTN::Vector<ParamDecl> __ParamDeclListType;
			class CmdDecl
			{
			public:
				GINL CmdDecl& operator = (const CmdDecl& src)
				{
					index = src.index;
					cmd = src.cmd;
					desc = src.desc;
					type = src.type;
					min_param_size = src.min_param_size;
					max_param_size = src.max_param_size;
					paramdecls = src.paramdecls;
					return *this;
				}
				GCLASS_COMPARE2(cmd, min_param_size, CmdDecl)
			public:
				GAIA::NUM index;
				__StringType cmd;
				__StringType desc;
				CMD_TYPE type;
				__ParamDeclListType::_sizetype min_param_size;
				__ParamDeclListType::_sizetype max_param_size;
				__ParamDeclListType paramdecls;
			};
			typedef GAIA::CTN::Vector<CmdDecl> __CmdDeclListType;
			typedef GAIA::CTN::Vector<__StringType> __ParamListType;
			class Cmd
			{
			public:
				GINL Cmd& operator = (const Cmd& src)
				{
					index = src.index;
					cmd = src.cmd;
					params = src.params;
					return *this;
				}
				GCLASS_COMPARE(cmd, Cmd)
				GAIA::NUM index;
				__StringType cmd;
				__ParamListType params;
			};
			typedef GAIA::CTN::Vector<Cmd> __CmdListType;
			typedef GAIA::CTN::Vector<GAIA::CTN::Ref<CmdDecl> > __CmdDeclOPTListType;
			typedef GAIA::CTN::Vector<GAIA::CTN::Ref<Cmd> > __CmdOPTListType;
			class CmdMutex
			{
			public:
				GINL CmdMutex& operator = (const CmdMutex& src)
				{
					cmd1 = src.cmd1;
					cmd2 = src.cmd2;
					return *this;
				}
				GCLASS_COMPARE(cmd1, CmdMutex)
				__StringType cmd1;
				__StringType cmd2;
			};
			class CmdCoop
			{
			public:
				GINL CmdCoop& operator = (const CmdCoop& src)
				{
					cmd1 = src.cmd1;
					cmd2 = src.cmd2;
					return *this;
				}
				GCLASS_COMPARE(cmd1, CmdCoop)
				__StringType cmd1;
				__StringType cmd2;
			};
			typedef GAIA::CTN::Vector<CmdMutex> __CmdMutexListType;
			typedef GAIA::CTN::Vector<CmdCoop> __CmdCoopListType;

		private:
			GINL GAIA::GVOID init(){m_bBeginDecl = GAIA::False;}

		private:
			GAIA::BL m_bBeginDecl;
			__CmdDeclListType m_decls;
			__CmdDeclOPTListType m_opt_decls;
			__CmdMutexListType m_mutex_decls;
			__CmdCoopListType m_coop_decls;
			__CmdListType m_cmds;
			__CmdOPTListType m_opt_cmds;
		};
	}
}

#endif
