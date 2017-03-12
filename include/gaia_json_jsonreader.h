#ifndef 	__GAIA_JSON_JSONREADER_H__
#define 	__GAIA_JSON_JSONREADER_H__

#include "gaia_type.h"
#include "gaia_assert.h"
#include "gaia_algo_string.h"
#include "gaia_json_jsonbase.h"

namespace GAIA
{
	namespace JSON
	{
		template<typename _DataType, typename _SizeType, typename _DepthType, _DepthType _MaxDepth> class BasicJsonReader : public GAIA::Base
		{
		public:
			typedef _DataType _datatype;
			typedef _SizeType _sizetype;
			typedef _DepthType _depthtype;

		public:
			static const _DepthType _maxdepth = _MaxDepth;

		public:
			typedef BasicJsonReader<_DataType, _SizeType, _DepthType, _MaxDepth> __MyType;

		public:
			/*!
				@brief Constructor.
			*/
			GINL BasicJsonReader(){this->init();}

			/*!
				@brief Destructor.
			*/
			GINL ~BasicJsonReader(){}

			/*!
				@brief Reset current JsonReader to default state(LIKE A NEW OBJECT).
			*/
			GINL GAIA::GVOID Reset(){this->init();}

			/*!
				@brief

				@param
			*/
			GINL GAIA::GVOID SetBuffer(const GAIA::GVOID* p, _SizeType size)
			{
				if(p == m_pFront && size == m_size)
					return;
				if(p == GNIL)
				{
					GAST(size == 0);
					m_pFront = m_pBack = m_pCursor = GNIL;
					m_size = 0;
				}
				else
				{
					GAST(size > sizeof(_DataType));
					m_pFront = (const _DataType*)p;
					m_pBack = m_pFront + (size / sizeof(_DataType) - 1);
					m_pCursor = m_pFront;
					m_size = size;
				}
			}

			/*!
				@brief

				@param

				@return
			*/
			GINL const GAIA::GVOID* GetBuffer(_SizeType& size) const{size = m_size; return m_pFront;}

			/*!
				@brief

				@return
			*/
			GINL _SizeType GetBufferSize() const{return m_size;}

			/*!
				@brief

				@return
			*/
			GINL _SizeType GetReadSize() const{return (m_pCursor - m_pFront) * sizeof(_DataType);}

			/*!
				@brief

				@return
			*/
			GINL _SizeType GetRemainSize() const{return this->GetBufferSize() - this->GetReadSize();}

			/*!
				@brief

				@param

				@return

				@exception

				@remarks
			*/
			GINL const _DataType* Peek(GAIA::JSON::JSON_NODE& nt, _SizeType& nodenamelen)
			{

			}

			/*!
				@brief

				@param

				@return

				@exception

				@remarks
			*/
			GINL const _DataType* Begin(GAIA::JSON::JSON_NODE& nt, _SizeType& nodenamelen)
			{

			}

			/*!
				@brief

				@param

				@return

				@exception

				@remarks
			*/
			GINL GAIA::GVOID End()
			{

			}

			/*!
				@brief

				@param

				@return

				@exception

				@remarks
			*/
			GINL const _DataType* Read(GAIA::JSON::JSON_NODE& nt, _SizeType& nodenamelen)
			{

			}

		private:
			GINL GAIA::GVOID init()
			{
				m_pFront = m_pBack = m_pCursor = GNIL;
				m_size = 0;
			}

		private:
			const _DataType* m_pFront;
			const _DataType* m_pBack; // Last valid character.
			const _DataType* m_pCursor;
			_SizeType m_size; // Size in bytes.
		};
		class JsonReaderA : public BasicJsonReader<GAIA::CH, GAIA::NUM, GAIA::NUM, 32>{public:};
		class JsonReaderW : public BasicJsonReader<GAIA::WCH, GAIA::NUM, GAIA::NUM, 32>{public:};
	}
}

#endif