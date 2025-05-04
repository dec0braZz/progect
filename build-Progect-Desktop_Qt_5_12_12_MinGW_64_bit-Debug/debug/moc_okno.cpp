/****************************************************************************
** Meta object code from reading C++ file 'okno.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.12)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Progect/okno.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'okno.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.12. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_OKNO_t {
    QByteArrayData data[11];
    char stringdata0[122];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_OKNO_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_OKNO_t qt_meta_stringdata_OKNO = {
    {
QT_MOC_LITERAL(0, 0, 4), // "OKNO"
QT_MOC_LITERAL(1, 5, 12), // "openSettings"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 6), // "closed"
QT_MOC_LITERAL(4, 26, 15), // "onButtonHovered"
QT_MOC_LITERAL(5, 42, 15), // "onButtonClicked"
QT_MOC_LITERAL(6, 58, 11), // "showSetting"
QT_MOC_LITERAL(7, 70, 16), // "showGroupActions"
QT_MOC_LITERAL(8, 87, 18), // "updateWindowColors"
QT_MOC_LITERAL(9, 106, 10), // "background"
QT_MOC_LITERAL(10, 117, 4) // "text"

    },
    "OKNO\0openSettings\0\0closed\0onButtonHovered\0"
    "onButtonClicked\0showSetting\0"
    "showGroupActions\0updateWindowColors\0"
    "background\0text"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_OKNO[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    2,   55,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QColor, QMetaType::QColor,    9,   10,

       0        // eod
};

void OKNO::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<OKNO *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->openSettings(); break;
        case 1: _t->closed(); break;
        case 2: _t->onButtonHovered(); break;
        case 3: _t->onButtonClicked(); break;
        case 4: _t->showSetting(); break;
        case 5: _t->showGroupActions(); break;
        case 6: _t->updateWindowColors((*reinterpret_cast< const QColor(*)>(_a[1])),(*reinterpret_cast< const QColor(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (OKNO::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OKNO::openSettings)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (OKNO::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&OKNO::closed)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject OKNO::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_OKNO.data,
    qt_meta_data_OKNO,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *OKNO::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *OKNO::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_OKNO.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int OKNO::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void OKNO::openSettings()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void OKNO::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
