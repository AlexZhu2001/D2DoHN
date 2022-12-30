#ifndef MYHIGHLIGHTER_H
#define MYHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class MyHighlighter : public QSyntaxHighlighter
{
public:
    MyHighlighter(QObject* parent);
    MyHighlighter(QTextDocument* parent);

    // QSyntaxHighlighter interface
protected:
    virtual void highlightBlock(const QString& text) Q_DECL_OVERRIDE;
};

#endif // MYHIGHLIGHTER_H
