#ifndef MEASE_DATASTREAM_P_HPP
#define MEASE_DATASTREAM_P_HPP

#define STREAM_RET(TYPE, VAR, ERROR, MSG)                                                                                                                      \
    const auto VAR = read<TYPE>(&ERROR);                                                                                                                       \
    if (ERROR.error != MEASE::DataStream::Error::NoError) {                                                                                                    \
        ERROR.errorString = QStringLiteral("%1: %2").arg(MSG, ERROR.errorString);                                                                              \
        return {};                                                                                                                                             \
    }

#define STREAM_RET_LEN(TYPE, VAR, LEN, ERROR, MSG)                                                                                                             \
    const auto VAR = read<TYPE>(LEN, &ERROR);                                                                                                                  \
    if (ERROR.error != MEASE::DataStream::Error::NoError) {                                                                                                    \
        ERROR.errorString = QStringLiteral("%1: %2").arg(MSG, ERROR.errorString);                                                                              \
        return {};                                                                                                                                             \
    }

#define STREAM_RET_COND(COND, P_ERROR, ERROR, MSG)                                                                                                             \
    if (COND) {                                                                                                                                                \
        if (P_ERROR) {                                                                                                                                         \
            *P_ERROR = MEASE::DataStream::Error{ERROR, MSG};                                                                                                   \
        }                                                                                                                                                      \
        return {};                                                                                                                                             \
    }

#endif // MEASE_DATASTREAM_P_HPP
