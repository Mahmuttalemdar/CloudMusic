﻿#include "audiodata.h"

#include <flac/flacfile.h>
#include <mpeg/mpegfile.h>
#include <mpeg/id3v2/id3v2tag.h>
#include <mpeg/id3v2/attachedpictureframe.h>
#include <tstring.h>
#include <QFileInfo>
#include <QImage>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
AudioData::AudioData(QObject *parent) : QObject(parent)
{

}

void AudioData::addFile(const QString &url)
{
    if (url.isEmpty()) {
        emit parseFinished(QString());
    }
    else {
        addFiles(QStringList() << url);
    }
}

void AudioData::addFiles(const QStringList &urls)
{
    if (urls.isEmpty()) {
        emit parseFinished(QString());
        return ;
    }

    QJsonObject rootJsonObject;
    QJsonArray array;
    rootJsonObject.insert("songsCount", urls.count());



    foreach (const QString& url , urls) {
        QFileInfo info(url);
        if (url.isEmpty() || !info.isFile())
            continue;

        auto file = new TagLib::MPEG::File(url.toLocal8Bit().constData());
        TagLib::ID3v2::Tag *tag2 = file->ID3v2Tag();
        QString suffix = info.suffix();

        // 获取音频信息
        if (file && file->isOpen()) {
            // 获取id3v1tag
            auto tag = file->tag();
            // 为了获取时长
            auto property = file->audioProperties();
            QString title = tag->title().toCString(true);
            if (title.isEmpty())
                title = info.baseName().toLower();
            QJsonObject object;
            object.insert("url", QString("file:///") + info.absoluteFilePath());
            object.insert("size", info.size());
            object.insert("title", title);
            object.insert("album", tag->album().toCString(true));
            object.insert("artist", tag->artist().toCString(true));
            object.insert("duration", property->lengthInSeconds());

            QString picUrl = QCoreApplication::applicationDirPath() + "/";
            if (tag2) {
                auto list = tag2->frameListMap()["APIC"];
                if (!list.isEmpty()) {
                    auto pic = reinterpret_cast<TagLib::ID3v2::AttachedPictureFrame *>(list.front());
                    if (pic && !pic->picture().isNull()) {
                        QImage image = QImage::fromData(QByteArray(pic->picture().data(), pic->picture().size()));
                        picUrl = picUrl + title + ".jpg";
                        if (image.save(picUrl/*picUrl + title + ".jpg"*/))
                            picUrl = "file:///" + picUrl;
                        else
                            picUrl = "";
                    }
                }
            }

//            picUrl = "file:///" + picUrl + "/";
//            QDir dir;
//            dir.mkdir(picUrl + "/images");
//            picUrl = QString("file:///") + picUrl + QString("/images/");
            object.insert("pic", picUrl);
            array.push_back(object);

            delete file;
            file = Q_NULLPTR;
            tag2 = Q_NULLPTR;
        }
    }

    rootJsonObject.insert("songs", array);
    QJsonDocument document;
    document.setObject(rootJsonObject);
    emit parseFinished(document.toJson());
}

void AudioData::addFiles(const QList<QUrl> &urls)
{
    QStringList lists;
    foreach (QUrl url, urls)
        lists << url.toLocalFile();
    addFiles(lists);
}

//void AudioData::addFiles(const QStringList &urls)
//{
//    if (urls.isEmpty()) {
//        emit parseFinished(QString());
//        return;
//    }

//    QJsonObject rootJsonObject;
//    QJsonArray array;
//    rootJsonObject.insert("songsCount", urls.count());

//    TagLib::File *file = Q_NULLPTR;
//    TagLib::ID3v2::Tag *tag2 = Q_NULLPTR;

//    foreach (const QString& url , urls) {
//        QFileInfo info(url);
//        if (url.isEmpty() || !info.isFile())
//            continue;

//        QString suffix = info.suffix();
//        if (suffix == "mp3") {
//            file = new TagLib::MPEG::File(url.toLocal8Bit().constData());
//            tag2 = static_cast<TagLib::MPEG::File*>(file)->ID3v2Tag();
//        }
//        else if (suffix == "flac") {
//            file = new TagLib::FLAC::File(url.toLocal8Bit().constData());
//            tag2 = static_cast<TagLib::FLAC::File*>(file)->ID3v2Tag();
//        }
//        // 获取音频信息
//        if (file && file->isOpen()) {
//            // 获取id3v1tag
//            auto tag = file->tag();
//            // 为了获取时长
//            auto property = file->audioProperties();
//            QString title = tag->title().toCString(true);
//            QJsonObject object;
//            object.insert("url", info.absoluteFilePath());
//            object.insert("size", info.size());
//            object.insert("title", title);
//            object.insert("album", tag->album().toCString(true));
//            object.insert("artist", tag->artist().toCString(true));
//            object.insert("duration", property->lengthInMilliseconds());

//            if (tag2) {
//                auto list = tag2->frameListMap()["APIC"];
//                if (!list.isEmpty()) {
//                    auto pic = reinterpret_cast<TagLib::ID3v2::AttachedPictureFrame *>(list.front());
//                    if (pic && !pic->picture().isNull()) {
//                        QImage image = QImage::fromData(QByteArray(pic->picture().data(), pic->picture().size()));
//                        image.save(title + ".jpg");
//                    }
//                }
//            }
//            object.insert("pic", title.isEmpty() ? "" : title + ".jpg");
//            array.push_back(object);

//            delete file;
//            file = Q_NULLPTR;
//            tag2 = Q_NULLPTR;
//        }
//    }

//    rootJsonObject.insert("songs", array);
//    QJsonDocument document;
//    document.setObject(rootJsonObject);
//    emit parseFinished(document.toJson());
//}

