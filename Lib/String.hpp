template<typename ...TS>
void String::append( TS const & ...vs )
{
    auto maxLen = Utils::getFillMaxLength( vs... ) + 1;
    if( _dataLen + maxLen > _bufLen )
        reserve( (int)Utils::round2n( _dataLen + maxLen ) );
    _dataLen += Utils::fill( _buf + _dataLen, vs... );
}

template<typename ...TS>
String String::make( TS const & ...vs )
{
    String rtv;
    rtv.append( vs... );
    return rtv;
}

