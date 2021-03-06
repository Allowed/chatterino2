#include "messagebuilder.hpp"
#include "colorscheme.hpp"
#include "emotemanager.hpp"
#include "resources.hpp"

namespace chatterino {
namespace messages {

MessageBuilder::MessageBuilder()
    : _words()
{
    _parseTime = std::chrono::system_clock::now();
}

SharedMessage MessageBuilder::build()
{
    return SharedMessage(new Message(this->originalMessage, _words));
}

void MessageBuilder::appendWord(const Word &word)
{
    _words.push_back(word);
}

void MessageBuilder::appendTimestamp()
{
    time_t t;
    time(&t);
    appendTimestamp(t);
}

void MessageBuilder::appendTimestamp(time_t time)
{
    char timeStampBuffer[69];

    // TODO(pajlada): Fix this
    QColor systemMessageColor(140, 127, 127);
    // QColor &systemMessageColor = ColorScheme::getInstance().SystemMessageColor;

    // Add word for timestamp with no seconds
    strftime(timeStampBuffer, 69, "%H:%M", localtime(&time));
    QString timestampNoSeconds(timeStampBuffer);
    appendWord(Word(timestampNoSeconds, Word::TimestampNoSeconds, systemMessageColor, QString(),
                    QString()));

    // Add word for timestamp with seconds
    strftime(timeStampBuffer, 69, "%H:%M:%S", localtime(&time));
    QString timestampWithSeconds(timeStampBuffer);
    appendWord(Word(timestampWithSeconds, Word::TimestampWithSeconds, systemMessageColor, QString(),
                    QString()));
}

QString MessageBuilder::matchLink(const QString &string)
{
    // TODO: Implement this xD
    return QString();
}

}  // namespace messages
}  // namespace chatterino
