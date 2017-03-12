#ifndef 	__GAIA_XML_XMLWRITER_H__
#define 	__GAIA_XML_XMLWRITER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"
#include "gaia_xml_xmlbase.h"

namespace GAIA
{
	namespace XML
	{
		/*!
			@brief Basic xml writer.
				It used for high performance xml generate.
		*/
		template<typename _DataType, typename _SizeType, typename _DepthType, _DepthType _MaxDepth> class BasicXmlWriter : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _DepthType _depthtype;

		public:
			static const _DepthType _maxdepth = _MaxDepth;

		public:
			typedef BasicXmlWriter<_DataType, _SizeType, _DepthType, _MaxDepth> __MyType;

		public:

			/*!
				@brief Constructor.
			*/
			GINL BasicXmlWriter(){this->init();}

			/*!
				@brief Destructor.
			*/
			GINL ~BasicXmlWriter(){}

			/*!
				@brief Reset current XmlWriter to default state(LIKE A NEW OBJECT).
			*/
			GINL GAIA::GVOID Reset(){this->init();}

			/*!
				@brief Set or unset a buffer for XmlWriter write to.

				@param p [in] Specify a new buffer.
					If GNIL, means unset the old buffer.

				@param size [in] Specify parameter p's size in bytes.
					If parameter p is GNIL, parameter size must be 0, or must above zero.

				@remarks
					Call SetBuffer will cause old xml data lost in old buffer.
			*/
			GINL GAIA::GVOID SetBuffer(const GAIA::GVOID* p, _SizeType size)
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
				m_LastNNVT = GAIA::XML::XML_NODE_INVALID;
			}

			/*!
				@brief Get the buffer which used for XmlWriter write to.

				@param size [out] Used for saving the size of buffer(return) in bytes.

				@return Return the buffer which had be set.
					If not call SetBuffer with a buffer pointer, will return GNIL, and parameter size be filled by 0.
			*/
			GINL const GAIA::GVOID* GetBuffer(_SizeType& size) const{size = m_size; return m_pFront;}

			/*!
				@brief Get the buffer's size in bytes which had be set to current XmlWriter.

				@return Return the buffer's size in bytes.
			*/
			GINL _SizeType GetBufferSize() const{return m_size;}

			/*!
				@brief Get the buffer's writen size in bytes.

				@return Return the buffer's size in bytes.
			*/
			GINL _SizeType GetWriteSize() const{return (m_pCursor - m_pFront) * sizeof(_DataType);}

			/*!
				@brief Get the buffer's remain size in bytes.

				@return Return the buffer's remain size in bytes.
			*/
			GINL _SizeType GetRemainSize() const{return this->GetBufferSize() - this->GetWriteSize();}

			/*!
				@brief Begin write a container node or a multi container node.

				@param nt [in] Specify xml node type, must be GAIA::XML::XML_NODE_CONTAINER or GAIA::XML::XML_NODE_MULTICONTAINER.

				@param pszNodeName [in] Specify the node name.

				@param nodenamelen [in] Specify the parameter pszNodeName's length in characters with out '\0'.
					It could be GINVALID means all parameter pszNodeName's character are used until to '\0'.

				@exception GAIA::ECT::EctInvalidParam
					If parameter nt is not GAIA::XML::XML_NODE_CONTAINER and is not GAIA::XML::XML_NODE_MULTICONTAINER, throw it.

				@exception GAIA::ECT::EctInvalidParam
					If parameter pszNodeName is not GNIL and pszNodeName is not a valid xml node name, throw it.

				@exception
					GAIA::ECT::EctIllegal If the last value node is not writen.
			*/
			GAIA::GVOID Begin(GAIA::XML::XML_NODE nt, const GAIA::CH* pszNodeName = GNIL, _SizeType nodenamelen = GINVALID)
			{
				if(nt != GAIA::XML::XML_NODE_CONTAINER &&
				   nt != GAIA::XML::XML_NODE_MULTICONTAINER)
					GTHROW(InvalidParam);
				if(pszNodeName != GNIL)
				{
					if(!GAIA::XML::XmlCheckNodeName(nt, pszNodeName))
						GTHROW(InvalidParam);
				}
				if(nodenamelen == GINVALID && pszNodeName != GNIL)
					nodenamelen = GAIA::ALGO::gstrlen(pszNodeName);
				if(m_LastNNVT == GAIA::XML::XML_NODE_NAME)
					GTHROW(Illegal);
				switch(nt)
				{
				case GAIA::XML::XML_NODE_CONTAINER:
					{
						if(m_CNTCursor == GINVALID)
						{
							GAST(!GAIA::ALGO::gstremp(pszNodeName));
							m_CNTCursor = 0;
							m_LastCNT[m_CNTCursor] = nt;
							this->write("<", sizeof("<") - 1);
							NodeName& nn = m_NodeNameStack[m_CNTCursor];
							nn.p = m_pCursor;
							nn.len = nodenamelen;
							this->write(pszNodeName, nodenamelen);

						}
						else
						{
							GAST(!GAIA::ALGO::gstremp(pszNodeName));
							GAST(m_CNTCursor + 1 < _MaxDepth);
							m_LastCNT[++m_CNTCursor] = nt;
							this->write("<", sizeof("<") - 1);
							NodeName& nn = m_NodeNameStack[m_CNTCursor];
							nn.p = m_pCursor;
							nn.len = nodenamelen;
							this->write(pszNodeName, nodenamelen);
						}
					}
					break;
				case GAIA::XML::XML_NODE_MULTICONTAINER:
					{
						if(m_CNTCursor == GINVALID)
						{
							GAST(!GAIA::ALGO::gstremp(pszNodeName));
							m_CNTCursor = 0;
							m_LastCNT[m_CNTCursor] = nt;
							this->write("<", sizeof("<") - 1);
							NodeName& nn = m_NodeNameStack[m_CNTCursor];
							nn.p = m_pCursor;
							nn.len = nodenamelen;
							this->write(pszNodeName, nodenamelen);
							this->write(">", sizeof(">") - 1);
						}
						else
						{
							GAST(!GAIA::ALGO::gstremp(pszNodeName));
							GAST(m_CNTCursor + 1 < _MaxDepth);
							m_LastCNT[++m_CNTCursor] = nt;
							this->write("<", sizeof("<") - 1);
							NodeName& nn = m_NodeNameStack[m_CNTCursor];
							nn.p = m_pCursor;
							nn.len = nodenamelen;
							this->write(pszNodeName, nodenamelen);
							this->write(">", sizeof(">") - 1);
						}
					}
					break;
				case GAIA::XML::XML_NODE_NAME:
					GASTFALSE;
					break;
				case GAIA::XML::XML_NODE_VALUE:
					GASTFALSE;
					break;
				default:
					GASTFALSE;
					break;
				}
				m_LastNNVT = GAIA::XML::XML_NODE_INVALID;
			}

			/*!
				@brief End write a container node or a multi container node.

				@exception GAIA::ECT::EctIllegal
					If the node not began, throw it.

				@exception GAIA::ECT::EctIllegal
					If the last value node is not writen, throw it.
			*/
			GINL GAIA::GVOID End()
			{
				if(m_CNTCursor == GINVALID)
					GTHROW(Illegal);
				if(m_LastNNVT == GAIA::XML::XML_NODE_NAME)
					GTHROW(Illegal);
				switch(m_LastCNT[m_CNTCursor])
				{
				case GAIA::XML::XML_NODE_CONTAINER:
					{
						this->write("/>", sizeof("/>") - 1);
					}
					break;
				case GAIA::XML::XML_NODE_MULTICONTAINER:
					{
						this->write("</", sizeof("</") - 1);
						NodeName& nn = m_NodeNameStack[m_CNTCursor];
						this->write(nn.p, nn.len);
						this->write(">", sizeof(">") - 1);
					}
					break;
				case GAIA::XML::XML_NODE_NAME:
					GASTFALSE;
					break;
				case GAIA::XML::XML_NODE_VALUE:
					GASTFALSE;
					break;
				default:
					GASTFALSE;
					break;
				}
				m_CNTCursor--;
				m_LastNNVT = GAIA::XML::XML_NODE_INVALID;
			}

			/*!
				@brief Write a name node or a value node.

				@param nt [in] Specify the node type, must be GAIA::XML::XML_NODE_NAME or GAIA::XML::XML_NODE_VALUE.

				@param pszNodeName [in] Specify the node name.

				@param nodenamelen [in] Specify the parameter pszNodeName's length in characters with out '\0'.
					It could be GINVALID means all parameter pszNodeName's character are used until to '\0'.

				@exception GAIA::ECT::EctInvalidParam
					If parameter nt is not GAIA::XML::XML_NODE_NAME and is not GAIA::XML::XML_NODE_VALUE, throw it.

				@exception GAIA::ECT::EctInvalidParam
					If parameter pszNodeName is GNIL or "", throw it.

				@exception GAIA::ECT::EctInvalidParam
					If parameter pszNodeName is not a valid xml node name, throw it.

				@exception GAIA::ECT::EctIllegal
					If have not call XmlWriter::Begin member function to begin a container or a multi container, throw it.

				@exception GAIA::ECT::EctIllegal
					If want write a value node, but the last node is not a name node.

				@exception GAIA::ECT::EctIllegal
					If want write a name not, but the last node is a name node.
			*/
			template<typename _ParamDataType>  GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const _ParamDataType* pszNodeName, _SizeType nodenamelen = GINVALID)
			{
				if(nt != GAIA::XML::XML_NODE_NAME && nt != GAIA::XML::XML_NODE_VALUE)
					GTHROW(InvalidParam);
				if(GAIA::ALGO::gstremp(pszNodeName))
					GTHROW(InvalidParam);
				if(!GAIA::XML::XmlCheckNodeName(nt, pszNodeName))
					GTHROW(InvalidParam);
				if(m_CNTCursor == GINVALID)
					GTHROW(Illegal);
				if(nodenamelen == GINVALID && pszNodeName != GNIL)
					nodenamelen = GAIA::ALGO::gstrlen(pszNodeName);
				switch(nt)
				{
				case GAIA::XML::XML_NODE_CONTAINER:
					GASTFALSE;
					break;
				case GAIA::XML::XML_NODE_MULTICONTAINER:
					GASTFALSE;
					break;
				case GAIA::XML::XML_NODE_NAME:
					{
						if(m_LastNNVT != GAIA::XML::XML_NODE_INVALID &&
						   m_LastNNVT != GAIA::XML::XML_NODE_VALUE)
							GTHROW(Illegal);
						this->write(" ", sizeof(" ") - 1);
						this->write(pszNodeName, nodenamelen);
						this->write("=\"", sizeof("=\"") - 1);
						m_LastNNVT = GAIA::XML::XML_NODE_NAME;
					}
					break;
				case GAIA::XML::XML_NODE_VALUE:
					{
						if(m_LastNNVT != GAIA::XML::XML_NODE_NAME)
							GTHROW(Illegal);
						this->write(pszNodeName, nodenamelen);
						this->write("\"", sizeof("\"") - 1);
						m_LastNNVT = GAIA::XML::XML_NODE_VALUE;
					}
					break;
				default:
					GASTFALSE;
					break;
				}
			}

			/*!
				@brief XmlWriter::Write's overwrite member function for write GAIA::BL type.
			*/
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::BL& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}

			/*!
				@brief XmlWriter::Write's overwrite member function for write GAIA::X128 type.
			*/
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::X128& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}

			/*!
				@brief XmlWriter::Write's overwrite member function for write GAIA::U8 type.
			*/
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::U8& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}

			/*!
				@brief XmlWriter::Write's overwrite member function for write GAIA::N8 type.
			*/
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::N8& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}

			/*!
				@brief XmlWriter::Write's overwrite member function for write GAIA::U16 type.
			*/
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::U16& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}

			/*!
				@brief XmlWriter::Write's overwrite member function for write GAIA::N16 type.
			*/
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::N16& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}

			/*!
				@brief XmlWriter::Write's overwrite member function for write GAIA::U32 type.
			*/
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::U32& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}

			/*!
				@brief XmlWriter::Write's overwrite member function for write GAIA::N32 type.
			*/
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::N32& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}

			/*!
				@brief XmlWriter::Write's overwrite member function for write GAIA::U64 type.
			*/
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::U64& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}

			/*!
				@brief XmlWriter::Write's overwrite member function for write GAIA::N64 type.
			*/
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::N64& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}

			/*!
				@brief XmlWriter::Write's overwrite member function for write GAIA::F32 type.
			*/
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::F32& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}

			/*!
				@brief XmlWriter::Write's overwrite member function for write GAIA::F64 type.
			*/
			GAIA::GVOID Write(GAIA::XML::XML_NODE nt, const GAIA::F64& v){GAIA::CH szTemp[64]; GAIA::ALGO::castv(v, szTemp, sizeof(szTemp)); this->Write(nt, szTemp);}

			/*!
				@brief Another method to call XmlWriter::Write member function will simpler form.

				@param psz [in] Specify a node's name or a node's value.

				@return Return current XmlWriter's reference.

				@remarks
					The parameter psz's node type(GAIA::XML::XML_NODE) will be decided by
					the times of write(XmlWriter's functions which have the write property) functions call,
					so, if current XmlWriter need a name string, you will write a name string by current function call,
					if current XmlWriter need a value string, you will write a value string by current function call.\n

					Notice that, current function can't write a GAIA::XML::XML_NODE_CONTAINER node or
					a GAIA::XML::XML_NODE_MULTICONTAINER, current function only write a GAIA::XML::XML_NODE_NAME or
					a GAIA::XML::XML_NODE_VALUE.\n
			*/
			__MyType& operator << (const GAIA::CH* psz)
			{
				if(m_LastNNVT == GAIA::XML::XML_NODE_INVALID || m_LastNNVT == GAIA::XML::XML_NODE_VALUE)
					this->Write(GAIA::XML::XML_NODE_NAME, psz);
				else if(m_LastNNVT == GAIA::XML::XML_NODE_NAME)
					this->Write(GAIA::XML::XML_NODE_VALUE, psz);
				else
					GASTFALSE;
				return *this;
			}

			/*!
				@brief Another method to call XmlWriter::Write member function will simpler form.

				@param psz [in] Specify a node's name or a node's value.

				@return Return current XmlWriter's reference.

				@remarks
					The parameter psz's node type(GAIA::XML::XML_NODE) will be decided by
					the times of write(XmlWriter's functions which have the write property) functions call,
					so, if current XmlWriter need a name string, you will write a name string by current function call,
					if current XmlWriter need a value string, you will write a value string by current function call.\n

					Notice that, current function can't write a GAIA::XML::XML_NODE_CONTAINER node or
					a GAIA::XML::XML_NODE_MULTICONTAINER, current function only write a GAIA::XML::XML_NODE_NAME or
					a GAIA::XML::XML_NODE_VALUE.\n
			*/
			__MyType& operator << (const GAIA::WCH* psz)
			{
				if(m_LastNNVT == GAIA::XML::XML_NODE_INVALID || m_LastNNVT == GAIA::XML::XML_NODE_VALUE)
					this->Write(GAIA::XML::XML_NODE_NAME, psz);
				else if(m_LastNNVT == GAIA::XML::XML_NODE_NAME)
					this->Write(GAIA::XML::XML_NODE_VALUE, psz);
				else
					GASTFALSE;
				return *this;
			}

			/*!
				@brief Another method to call XmlWriter::Write member function for write GAIA::BL.
			*/
			__MyType& operator << (const GAIA::BL& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}

			/*!
				@brief Another method to call XmlWriter::Write member function for write GAIA::X128.
			*/
			__MyType& operator << (const GAIA::X128& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}

			/*!
				@brief Another method to call XmlWriter::Write member function for write GAIA::U8.
			*/
			__MyType& operator << (const GAIA::U8& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}

			/*!
				@brief Another method to call XmlWriter::Write member function for write GAIA::N8.
			*/
			__MyType& operator << (const GAIA::N8& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}

			/*!
				@brief Another method to call XmlWriter::Write member function for write GAIA::U16.
			*/
			__MyType& operator << (const GAIA::U16& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}

			/*!
				@brief Another method to call XmlWriter::Write member function for write GAIA::N16.
			*/
			__MyType& operator << (const GAIA::N16& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}

			/*!
				@brief Another method to call XmlWriter::Write member function for write GAIA::U32.
			*/
			__MyType& operator << (const GAIA::U32& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}

			/*!
				@brief Another method to call XmlWriter::Write member function for write GAIA::N32.
			*/
			__MyType& operator << (const GAIA::N32& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}

			/*!
				@brief Another method to call XmlWriter::Write member function for write GAIA::U64.
			*/
			__MyType& operator << (const GAIA::U64& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}

			/*!
				@brief Another method to call XmlWriter::Write member function for write GAIA::N64.
			*/
			__MyType& operator << (const GAIA::N64& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}

			/*!
				@brief Another method to call XmlWriter::Write member function for write GAIA::F32.
			*/
			__MyType& operator << (const GAIA::F32& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}

			/*!
				@brief Another method to call XmlWriter::Write member function for write GAIA::F64.
			*/
			__MyType& operator << (const GAIA::F64& v){this->Write(GAIA::XML::XML_NODE_VALUE, v); return *this;}

		private:
			class NodeName : public GAIA::Base
			{
			public:
				_DataType* p;
				_SizeType len;
			};

		private:
			GINL GAIA::GVOID init()
			{
				GAST(_MaxDepth > 0);
				m_pFront = m_pBack = m_pCursor = GNIL;
				m_size = 0;
				m_CNTCursor = GINVALID;
				m_LastNNVT = GAIA::XML::XML_NODE_INVALID;
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
			GAIA::XML::XML_NODE m_LastCNT[_MaxDepth];
			_DepthType m_CNTCursor;
			GAIA::XML::XML_NODE m_LastNNVT; // NNVT means node name value type.
			NodeName m_NodeNameStack[_MaxDepth];
		};
		class XmlWriterA : public BasicXmlWriter<GAIA::CH, GAIA::NUM, GAIA::NUM, 32>{public:};
		class XmlWriterW : public BasicXmlWriter<GAIA::WCH, GAIA::NUM, GAIA::NUM, 32>{public:};
	}
}

#endif