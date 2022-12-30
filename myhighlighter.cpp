#include "myhighlighter.h"
#include "qregularexpression.h"

MyHighlighter::MyHighlighter(QObject* parent)
    : QSyntaxHighlighter(parent)
{
}

MyHighlighter::MyHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
}

void
MyHighlighter::highlightBlock(const QString& text)
{
    QTextCharFormat errFormat;
    errFormat.setForeground(QBrush(Qt::red));
    QTextCharFormat warnFormat;
    warnFormat.setForeground(QBrush(Qt::yellow));
    static const QRegularExpression errExp("(\\[error\\])|(!!!.*!!!)");
    static const QRegularExpression warnExp("\\[warn\\]");
    auto iErr = errExp.globalMatch(text);
    auto iWarn = warnExp.globalMatch(text);
    while (iErr.hasNext()) {
        auto m = iErr.next();
        setFormat(m.capturedStart(), m.capturedLength(), errFormat);
    }
    while (iWarn.hasNext()) {
        auto m = iWarn.next();
        setFormat(m.capturedStart(), m.capturedLength(), errFormat);
    }
}
