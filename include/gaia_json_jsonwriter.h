#ifndef 	__GAIA_JSON_JSONWRITER_H__
#define 	__GAIA_JSON_JSONWRITER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"
#include "gaia_json_jsonbase.h"

namespace GAIA
{
	namespace JSON
	{
		template<typename _DataType, typename _SizeType, typename _DepthType, _DepthType _MaxDepth> class BasicJsonWriter : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _DepthType _depthtype;

		public:
			static const _DepthType _maxdepth = _MaxDepth;

		public:
			typedef BasicJsonWriter<_DataType, _SizeType, _DepthType, _MaxDepth> __MyType;

		public:
			GINL BasicJsonWriter(){this->init();}
			GINL ~BasicJsonWriter(){}

			GINL GAIA::GVOID SetBuffer(GAIA::GVOID* p, _SizeType size)
			{
				if(p == GNIL)
				{
					GAST(size == 0);
					m_pFront = m_pBack = m_pCursor = GNIL;
					m_size = 0;
				}
				else
				{
					GAST(size > sizeof(_DataType));
					m_pFront = (_DataType*)p;
					m_pBack = m_pFront + (size / sizeof(_DataType) - 1);
					m_pCursor = m_pFront;
					m_size = size;
				}
				m_CNTCursor = GINVALID;
				m_LastNNVT = GAIA::JSON::JSON_NODE_INVALID;
				m_bFirstNode = GAIA::True;
			}
			GINL GAIA::GVOID* GetBuffer(_SizeType& size) const{size = m_size; return m_pFront;}
			GINL _SizeType GetBufferSize() const{return m_size;}
			GINL _SizeType GetWriteSize() const{return (m_pCursor - m_pFront) * sizeof(_DataType);}
			GINL _SizeType GetRemainSize() const{return this->GetBufferSize() - this->GetWriteSize();}

			GAIA::GVOID Begin(GAIA::JSON::JSON_NODE nt, const GAIA::CH* pszNodeName = GNIL, _SizeType nodenamelen = GINVALID)
			{
				GAST(nt > GAIA::JSON::JSON_NODE_INVALID && nt < GAIA::JSON::JSON_NODE_MAXENUMCOUNT);
				if(pszNodeName != GNIL)
				{
					if(!GAIA::JSON::JsonCheckNodeName(nt, pszNodeName))
						GTHROW(InvalidParam);
				}
				if(nodenamelen == GINVALID && pszNodeName != GNIL)
					nodenamelen = GAIA::ALGO::gstrlen(pszNodeName);
				if(m_LastNNVT == GAIA::JSON::JSON_NODE_NAME)
					GTHROW(Illegal);
				switch(nt)
				{
				case GAIA::JSON::JSON_NODE_CONTAINER:
					{
						if(m_CNTCursor == GINVALID)
						{
							GAST(GAIA::ALGO::gstremp(pszNodeName));
							m_CNTCursor = 0;
							m_LastCNT[m_CNTCursor] = nt;
							if(!m_bFirstNode)
								this->write(",", sizeof(",") - 1);
							this->write("{", sizeof("{") - 1);

						}
						else
						{
							GAST(!GAIA::ALGO::gstremp(pszNodeName));
							GAST(m_CNTCursor + 1 < _MaxDepth);
							GAIA::JSON::JSON_NODE parentnt = m_LastCNT[m_CNTCursor];
							m_LastCNT[++m_CNTCursor] = nt;
							if(!m_bFirstNode)
								this->write(",", sizeof(",") - 1);
							if(parentnt == GAIA::JSON::JSON_NODE_MULTICONTAINER)
								this->write("{\"", sizeof("{\"") - 1);
							else
								this->write("\"", sizeof("\"") - 1);
							this->write(pszNodeName, nodenamelen);
							this->write("\":{", sizeof("\":{") - 1);
						}
					}
					break;
				case GAIA::JSON::JSON_NODE_MULTICONTAINER:
					{
						if(m_CNTCursor == GINVALID)
						{
							GAST(GAIA::ALGO::gstremp(pszNodeName));
							m_CNTCursor = 0;
							m_LastCNT[m_CNTCursor] = nt;
							if(!m_bFirstNode)
								this->write(",", sizeof(",") - 1);
							this->write("[", sizeof("[") - 1);
						}
						else
						{
							GAST(!GAIA::ALGO::gstremp(pszNodeName));
							GAST(m_CNTCursor + 1 < _MaxDepth);
							GAIA::JSON::JSON_NODE parentnt = m_LastCNT[m_CNTCursor];
							m_LastCNT[++m_CNTCursor] = nt;
							if(!m_bFirstNode)
								this->write(",", sizeof(",") - 1);
							if(parentnt == GAIA::JSON::JSON_NODE_MULTICONTAINER)
								this->write("{\"", sizeof("{\"") - 1);
							else
								this->write("\"", sizeof("\"") - 1);
							this->write(pszNodeName, nodenamelen);
							this->write("\":[", sizeof("\":[") - 1);
						}
					}
					break;
				case GAIA::JSON::JSON_NODE_NAME:
					GTHROW(Illegal);
					break;
				case GAIA::JSON::JSON_NODE_VALUE:
					GTHROW(Illegal);
					break;
				default:
					GASTFALSE;
					break;
				}
				m_LastNNVT = GAIA::JSON::JSON_NODE_INVALID;
				m_bFirstNode = GAIA::True;
			}
			GINL GAIA::GVOID End()
			{
				if(m_CNTCursor == GINVALID)
					GTHROW(Illegal);
				if(m_LastNNVT == GAIA::JSON::JSON_NODE_NAME)
					GTHROW(Illegal);
				switch(m_LastCNT[m_CNTCursor])
				{
				case GAIA::JSON::JSON_NODE_CONTAINER:
					{
						if(m_CNTCursor > 0 && m_LastCNT[m_CNTCursor - 1] == GAIA::JSON::JSON_NODE_MULTICONTAINER)
							this->write("}}", sizeof("}}") - 1);
						else
							this->write("}", sizeof("}") - 1);
					}
					break;
				case GAIA::JSON::JSON_NODE_MULTICONTAINER:
					{
						if(m_CNTCursor > 0 && m_LastCNT[m_CNTCursor - 1] == GAIA::JSON::JSON_NODE_MULTICONTAINER)
							this->write("}]", sizeof("}]") - 1);
						else
							this->write("]", sizeof("]") - 1);
					}
					break;
				case GAIA::JSON::JSON_NODE_NAME:
					GTHROW(Illegal);
					break;
				case GAIA::JSON::JSON_NODE_VALUE:
					GTHROW(Illegal);
					break;
				default:
					GASTFALSE;
					break;
				}
				m_CNTCursor--;
				m_LastNNVT = GAIA::JSON::JSON_NODE_INVALID;
				m_bFirstNode = GAIA::False;
			}
			template<typename _ParamDataType> GAIA::GVOID Write(GAIA::JSON::JSON_NODE nt, const _ParamDataType* pszNodeName, _SizeType nodenamelen = GINVALID)
			{
				GAST(nt > GAIA::JSON::JSON_NODE_INVALID && nt < GAIA::JSON::JSON_NODE_MAXENUMCOUNT);
				GAST(!GAIA::ALGO::gstremp(pszNodeName));
				if(!GAIA::JSON::JsonCheckNodeName(nt, pszNodeName))
					GTHROW(InvalidParam);
				if(m_CNTCursor == GINVALID)
					GTHROW(Illegal);
				if(m_LastCNT[m_CNTCursor] != GAIA::JSON::JSON_NODE_CONTAINER)
					GTHROW(Illegal);
				if(nodenamelen == GINVALID && pszNodeName != GNIL)
					nodenamelen = GAIA::ALGO::gstrlen(pszNodeName);
				switch(nt)
				{
				case GAIA::JSON::JSON_NODE_CONTAINER:
					GTHROW(Illegal);
					break;
				case GAIA::JSON::JSON_NODE_MULTICONTAINER:
					GTHROW(Illegal);
					break;
				case GAIA::JSON::JSON_NODE_NAME:
					{
						if(m_LastNNVT != GAIA::JSON::JSON_NODE_INVALID &&
						   m_LastNNVT != GAIA::JSON::JSON_NODE_VALUE)
							GTHROW(Illegal);
						if(!m_bFirstNode)
							this->write(",", sizeof(",") - 1);
						else
							m_bFirstNode = GAIA::False;
						this->write("\"", sizeof("\"") - 1);
						this->write(pszNodeName, nodenamelen);
						this->write("\":", sizeof("\":") - 1);
						m_LastNNVT = GAIA::JSON::JSON_NODE_NAME;
					}
					break;
				case GAIA::JSON::JSON_NODE_VALUE:
					{
						if(m_LastNNVT != GAIA::JSON::JSON_NODE_NAME)
							GTHROW(Illegal);
						this->write("\"", sizeof("\"") - 1);
						this->write(pszNodeName, nodenamelen);
						this->write("\"", sizeof("\"") - 1);
						m_LastNNVT = GAIA::JSON::JSON_NODE_VALUE;
					}
					break;
				default:
					GASTFALSE;
					break;
				}
			}
			GAIA::GVOID Write(GAIA::JSON::JSON_NODE nt, const GAIA::BL& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::JSON::JSON_NODE nt, const GAIA::X128& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::JSON::JSON_NODE nt, const GAIA::U8& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::JSON::JSON_NODE nt, const GAIA::N8& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::JSON::JSON_NODE nt, const GAIA::U16& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::JSON::JSON_NODE nt, const GAIA::N16& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::JSON::JSON_NODE nt, const GAIA::U32& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::JSON::JSON_NODE nt, const GAIA::N32& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::JSON::JSON_NODE nt, const GAIA::N64& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::JSON::JSON_NODE nt, const GAIA::U64& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::JSON::JSON_NODE nt, const GAIA::F32& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			GAIA::GVOID Write(GAIA::JSON::JSON_NODE nt, const GAIA::F64& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}
			__MyType& operator << (const GAIA::CH* p)
			{
				if(m_LastNNVT == GAIA::JSON::JSON_NODE_INVALID || m_LastNNVT == GAIA::JSON::JSON_NODE_VALUE)
					this->Write(GAIA::JSON::JSON_NODE_NAME, p);
				else if(m_LastNNVT == GAIA::JSON::JSON_NODE_NAME)
					this->Write(GAIA::JSON::JSON_NODE_VALUE, p);
				else
					GASTFALSE;
				return *this;
			}
			__MyType& operator << (const GAIA::WCH* p)
			{
				if(m_LastNNVT == GAIA::JSON::JSON_NODE_INVALID || m_LastNNVT == GAIA::JSON::JSON_NODE_VALUE)
					this->Write(GAIA::JSON::JSON_NODE_NAME, p);
				else if(m_LastNNVT == GAIA::JSON::JSON_NODE_NAME)
					this->Write(GAIA::JSON::JSON_NODE_VALUE, p);
				else
					GASTFALSE;
				return *this;
			}
			__MyType& operator << (const GAIA::BL& v){this->Write(GAIA::JSON::JSON_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::X128& v){this->Write(GAIA::JSON::JSON_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::U8& v){this->Write(GAIA::JSON::JSON_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::N8& v){this->Write(GAIA::JSON::JSON_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::U16& v){this->Write(GAIA::JSON::JSON_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::N16& v){this->Write(GAIA::JSON::JSON_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::U32& v){this->Write(GAIA::JSON::JSON_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::N32& v){this->Write(GAIA::JSON::JSON_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::U64& v){this->Write(GAIA::JSON::JSON_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::N64& v){this->Write(GAIA::JSON::JSON_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::F32& v){this->Write(GAIA::JSON::JSON_NODE_VALUE, v); return *this;}
			__MyType& operator << (const GAIA::F64& v){this->Write(GAIA::JSON::JSON_NODE_VALUE, v); return *this;}

		private:
			GINL GAIA::GVOID init()
			{
				GAST(_MaxDepth > 0);
				m_pFront = m_pBack = m_pCursor = GNIL;
				m_size = 0;
				m_CNTCursor = GINVALID;
				m_LastNNVT = GAIA::JSON::JSON_NODE_INVALID;
				m_bFirstNode = GAIA::True;
			}
			template<typename _ParamDataType> GAIA::GVOID write(const _ParamDataType* p, _SizeType size) // parameter size is valid character count.
			{
				GAST(p != GNIL);
				GAST(size > 0);
				if(size * sizeof(_DataType) > this->GetRemainSize())
					GTHROW(BufNotEnough);
				for(_SizeType x = 0; x < size; ++x)
					*m_pCursor++ = p[x];
			}

		private:
			_DataType* m_pFront;
			_DataType* m_pBack; // Last valid character.
			_DataType* m_pCursor;
			_SizeType m_size; // Size in bytes.
			GAIA::JSON::JSON_NODE m_LastCNT[_MaxDepth]; // CNT means container node type.
			_DepthType m_CNTCursor;
			GAIA::JSON::JSON_NODE m_LastNNVT; // NNVT means node name value type.
			GAIA::BL m_bFirstNode;
		};
		class JsonWriterA : public BasicJsonWriter<GAIA::CH, GAIA::NUM, GAIA::NUM, 32>{public:};
		class JsonWriterW : public BasicJsonWriter<GAIA::WCH, GAIA::NUM, GAIA::NUM, 32>{public:};
	}
}

#endif