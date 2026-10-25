// Creola GUI widgets. No Q_OBJECT classes are used (lambdas instead of slots), so no moc step is required.
#pragma once
#include<QtWidgets>
#include<functional>
#include<memory>

#include "completion.hpp"
#include "docs.hpp"
#include "plot.hpp"
#include "plot_qt.hpp"
#include "pretty.hpp"
#include "theme.hpp"
#include "vm.hpp"

// ----------------------------------------------------------------------------
// begin::namespace::creola::gui
// ----------------------------------------------------------------------------
namespace creola {
// -

inline QString q(const std::string& s);
inline std::string u8(const QString& s);

// State shared by all widgets
struct Context {
    VM* vm = nullptr;
    const Theme* theme = &lightTheme();
    QFont mono;
};

// -
class CodeHighlighter : public QSyntaxHighlighter {
public:
    CodeHighlighter(QTextDocument* doc, Context* ctx);

protected:
    void highlightBlock(const QString& text) override;


private:
    Context* m_ctx;
};

// -
class CellEdit : public QPlainTextEdit{
public:
    std::function<void(void)> onRun;
    std::function<void(int)> onZoom;

    explicit CellEdit(Context* ctx);

    void rehighlight(void);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    // -
    bool complete(bool);
    void applyCompletion(const QString& word);
    void recall(bool older);

    Context* m_ctx;
    CodeHighlighter* m_highlighter;
    QCompleter* m_completer;
    int m_compStart = 0;
};

// -
class Notebook : public QScrollArea{
public:
    std::function<void(const QString&, bool)> status;   // message, isError
    std::function<void(void)> afterRun;
    std::function<void(void)> changed;
    std::function<void(int)> zoom;

    explicit Notebook(Context* ctx);
    void addCell(const QString& text, int at=-1);
    void insertText(const QString6 text);
    void run(size_t i);
    void runAll(void);
    void rehighlightAll(void);
    void themeChanged(void);
    void fontChanged(void);
    QString toText(void) const;
    void fromText(QString& text);
    bool isBlank(void) const;
    QString currentWork(void) const;

private:
    struct Cell {
        CellEdit* in;
        QLabel* out;
        QFrame* box;
        bool err = false;
    };

    void styleOutput(Cell& cell);
    Context* m_ctx;
    QVBoxLayout* m_layout;
    std::vector<Cell> m_cells;
};


// -
class PlotWidget : public QWidget {
public:
    explicit PlotWidget(Context* ctx);
    void setFigure(const Figure& fig);
    bool hasFigure(void) const;
    const Figure& figure(void) const;

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    Context* m_ctx;
    Figure m_fig;
    bool m_has;
};

// --
class DocPanel : public QWidget {
public:
    explicit DocPanel(Context* ctx);
    void show(const std::string& name);
    void refresh(void);

private:
    void render(void);

    Context* m_ctx;
    QLineEdit* m_search;
    QListWidget* m_list;
    QTextBrowser* m_browser;
    std::vector<std::string> m_trail;
    int m_pos = -1;
};

inline void fillTable(QTableWidget* table, const Value& value);
inline void applyTheme(QApplication& app, const Theme& theme);

// ----------------------------------------------------------------------------
}// end::namespace::creola::gui
// ----------------------------------------------------------------------------