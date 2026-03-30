# AGENTS.md - Coding Guidelines for spotwebfc

## Project Overview

**Spotweb Filter Creator** - A Qt5-based GUI application for creating Spotweb filter files (.xml)

- **Language**: C++17
- **Framework**: Qt 5.15.2 (Widgets, GUI, XML, LinguistTools)
- **Build Systems**: CMake (primary), qmake (.pro file for Qt Creator)
- **Platform**: Cross-platform (Windows, Linux)

## Build Commands

### CMake (Recommended)

```bash
# Configure (Debug)
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# Configure (Release)
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release

# Test (if tests exist)
ctest -C Release --test-dir build
```

### qmake (Alternative)

```bash
# Generate Makefile
qmake spotwebfc.pro

# Build (Release)
make release

# Build (Debug)
make debug
```

### Windows (Visual Studio)

```bash
# Using CMake with MSVC
cmake -B build -G "Visual Studio 16 2019" -A x64
cmake --build build --config Release
```

## Code Style Guidelines

### File Headers

Every source file must include the GPL v3 license header:

```cpp
/*
 * Spotweb Filter Creator
 * Copyleft 2012
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
```

### Include Guards

Use `#ifndef`/`#define`/`#endif` pattern:

```cpp
#ifndef FILENAME_H
#define FILENAME_H
// ... content ...
#endif // FILENAME_H
```

### Naming Conventions

- **Classes**: `PascalCase` (e.g., `MainWidget`, `FilterTreeWidgetItem`)
- **Methods/Functions**: `camelCase` (e.g., `installTranslation`, `getName`)
- **Member Variables**: Hungarian notation with `m_` prefix (e.g., `m_pSettings`, `m_Name`)
- **Constants/Macro Definitions**: `UPPER_CASE_WITH_UNDERSCORES`
- **Qt Slots**: Prefix with `slot` (e.g., `slotOpenButtonClicked`)
- **UI Pointers**: Use `ui` for the UI pointer in QWidgets

### Imports

Order includes as follows:

```cpp
// 1. Corresponding header file (for .cpp files)
#include "mainwidget.h"
#include "ui_mainwidget.h"

// 2. Project headers (alphabetically)
#include "aboutdialog.h"
#include "newfilterdialog.h"

// 3. Qt headers (group by module, alphabetically within groups)
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore/QTextStream>
#include <QtGui/QCloseEvent>
#include <QtXml/QDomDocument>
```

### Type Declarations

- Use Qt types where available: `QString`, `QList`, `quint32`
- Pointer declarations: `Type* ptr` (asterisk with no space before)
- Reference declarations: `Type& ref` (ampersand with no space before)
- Const correctness: `const QString& param`

### Class Structure

```cpp
class MyClass : public QWidget
{
    Q_OBJECT

public:
    explicit MyClass(QWidget *parent = 0);
    ~MyClass();
    
    // Public methods
    
protected:
    // Protected methods/events
    void closeEvent(QCloseEvent* event);

private slots:
    // Qt slots (prefix with "slot")
    void slotButtonClicked();

private:
    // Member variables (m_ prefix)
    Ui::MyClass *ui;
    QSettings* m_pSettings;
    QString m_Name;
};
```

### Error Handling

- Check Qt pointer allocations: `if(m_pSettings)`
- Use `QMessageBox` for user-facing errors
- Return error codes from save operations (0 = success)
- Check file operations with `QFile::open()` before proceeding

### Qt-Specific Patterns

- **Signals/Slots**: Use `private slots:` section, prefix slot methods with `slot`
- **Translation**: Implement `changeEvent()` and `retranslate()` for i18n support
- **Memory Management**: Use parent-child hierarchy; explicit `delete` for non-Qt-owned pointers
- **UI Files**: Always include `ui_<filename>.h` after the main header

### Formatting

- 4-space indentation (no tabs)
- Opening brace on same line for methods, classes
- Spaces around operators: `if (condition)`
- Pointer/reference alignment: `Type* name`, `Type& name`
- Line length: ~100-120 characters preferred

## Project Structure

```
├── CMakeLists.txt          # CMake configuration
├── spotwebfc.pro            # qmake configuration
├── spotwebfc.qrc           # Qt resources
├── spotwebfc.cxml          # Content definitions (runtime required)
├── spotwebfc.lng           # Language definitions (runtime required)
├── *.ts                    # Translation files (Dutch, French, German)
├── main.cpp                # Application entry point
├── mainwidget.*            # Main window
├── *dialog.*               # Dialog classes
├── filtertreewidgetitem.*  # Custom tree widget item
└── .github/workflows/      # CI/CD (Windows & Ubuntu)
```

## CI/CD

GitHub Actions workflows:
- `cmake-windows-latest.yml`: Windows build with MSVC
- `cmake-ubuntu-latest.yml`: Ubuntu build with GCC

Both use Qt 5.15.2 and run on push/PR to `master` branch.

## Testing

No unit test framework is currently configured. Tests (if added) should use Qt Test:
- File naming: `test_<name>.cpp`
- Use `QTest` macros: `QCOMPARE`, ` QVERIFY`
- Add to CMake with `enable_testing()` and `add_test()`

## License

GPL v3 - All contributions must be compatible.
