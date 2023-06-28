#pragma once

_R2D_NAMESPACE_START_

template <class TVAL, class TSIG>
struct CacheGuard
{
public:
	CacheGuard(TVAL value, TSIG sign);
	CacheGuard(TSIG sign);
	CacheGuard();

	void update(TVAL value, TSIG sign);
	inline const TVAL getValue() const { return value; }
	inline const TSIG getSignature() const { return m_signature; }

private:
	TVAL m_value;
	TSIG m_signature;
};




template<class TVAL, class TSIG>
inline CacheGuard<TVAL, TSIG>::CacheGuard(TVAL value, TSIG sign)
	: m_value{value}, m_signature{sign}
{

}

template<class TVAL, class TSIG>
inline CacheGuard<TVAL, TSIG>::CacheGuard(TSIG sign)
	: m_value(), m_signature{sign}
{

}

template<class TVAL, class TSIG>
inline CacheGuard<TVAL, TSIG>::CacheGuard()
	: m_value(), m_signature()
{
}

template<class TVAL, class TSIG>
inline void CacheGuard<TVAL, TSIG>::update(TVAL value, TSIG sign)
{
	if (sign == m_signature)
		return;
	m_signature = sign;
	m_value = value;
}

_R2D_NAMESPACE_END_