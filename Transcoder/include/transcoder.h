#ifndef TRANSCODER_H
#define TRANSCODER_H

#include <QtCore/qglobal.h>
#include <QProcess>

#if defined(LIBRARY_TRANSCODER)
#define TRANSCODERSHARED_EXPORT Q_DECL_EXPORT
#else
#define TRANSCODERSHARED_EXPORT Q_DECL_IMPORT
#endif

class TRANSCODERSHARED_EXPORT Transcoder : public QProcess
{
    Q_OBJECT
public:
    Transcoder();

    void setInput(const QString &file);
    void setOutput(const QString &file);

    void setThreads(int threads);

    void setScale(int width = 0, int height = 0);

    void setVideoCodec(const QString &str);
    void setPreset(const QString &str);
    void setCRF(int crf);
    void setMaxrate(const QString &str);
    void setMinrate(const QString &str);

    void setAudioCodec(const QString &str);
    void setAudioBitrate(const QString &str);

    void transcode();

signals:
    void progress(int percent);

private slots:
    void message();
    void finish();

private:
    QString m_sInput;
    QString m_sOutput;

    int m_nScaleWidth;
    int m_nScaleHeight;

    int m_nThreads;

    QString m_sVCodec;
    QString m_sPreset;
    int m_nCRF;
    QString m_sMaxrate;
    QString m_sMinrate;

    QString m_sACodec;
    QString m_sABitrate;

    int m_nDuration;
};

inline void Transcoder::setInput(const QString &file)
{
    m_sInput = file;
}

inline void Transcoder::setOutput(const QString &file)
{
    m_sOutput = file;
}

inline void Transcoder::setThreads(int threads)
{
    m_nThreads = threads;
}

inline void Transcoder::setScale(int width, int height)
{
    m_nScaleWidth = width;
    m_nScaleHeight = height;
}

inline void Transcoder::setVideoCodec(const QString &str)
{
    m_sVCodec = str;
}

inline void Transcoder::setPreset(const QString &str)
{
    m_sPreset = str;
}

inline void Transcoder::setCRF(int crf)
{
    m_nCRF = crf;
}

inline void Transcoder::setMaxrate(const QString &str)
{
    m_sMaxrate = str;
}

inline void Transcoder::setMinrate(const QString &str)
{
    m_sMinrate = str;
}

inline void Transcoder::setAudioCodec(const QString &str)
{
    m_sACodec = str;
}

inline void Transcoder::setAudioBitrate(const QString &str)
{
    m_sABitrate = str;
}

#endif // TRANSCODER_H
