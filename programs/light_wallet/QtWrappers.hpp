#pragma once

#include <QDateTime>
#include <QLatin1String>
#include <QObject>
#include <QDebug>
#include <QQmlListProperty>

#include <bts/light_wallet/light_wallet.hpp>

#include "QtConversions.hpp"

class LedgerEntry : public QObject
{
   Q_OBJECT
   Q_PROPERTY(QString sender MEMBER m_sender NOTIFY stub)
   Q_PROPERTY(QString receiver MEMBER m_receiver NOTIFY stub)
   Q_PROPERTY(qreal amount MEMBER m_amount NOTIFY stub)
   Q_PROPERTY(QString symbol MEMBER m_symbol NOTIFY stub)
   Q_PROPERTY(QString memo MEMBER m_memo NOTIFY stub)

   QString m_sender;
   QString m_receiver;
   qreal m_amount;
   QString m_symbol;
   QString m_memo;

public:
   LedgerEntry(QObject* parent = nullptr): QObject(parent){}
   ~LedgerEntry(){}

Q_SIGNALS:
   //None of the properties change, so squelch warnings by setting this as their NOTIFY property
   void stub();
};

class TransactionSummary : public QObject
{
   Q_OBJECT
   Q_PROPERTY(QString id MEMBER m_id NOTIFY stub)
   Q_PROPERTY(QString timestamp MEMBER m_when NOTIFY stub)
   Q_PROPERTY(QString feeAmount MEMBER m_feeAmount NOTIFY stub)
   Q_PROPERTY(QString feeSymbol MEMBER m_feeSymbol NOTIFY stub)
   Q_PROPERTY(QQmlListProperty<LedgerEntry> ledger READ ledger CONSTANT)

public:
   // TransactionSummary takes ownership of the LedgerEntries in ledger
   TransactionSummary(QString id, QString timestamp, QList<LedgerEntry*>&& ledger, QObject* parent = nullptr);
   ~TransactionSummary(){}

   QQmlListProperty<LedgerEntry> ledger()
   {
      return QQmlListProperty<LedgerEntry>(this, m_ledger);
   }

private:
   QString m_id;
   QString m_when;
   QString m_feeAmount;
   QString m_feeSymbol;
   QList<LedgerEntry*> m_ledger;

Q_SIGNALS:
   //None of the properties change, so squelch warnings by setting this as their NOTIFY property
   void stub();
};

class Balance : public QObject
{
   Q_OBJECT
   Q_PROPERTY(QString symbol MEMBER m_symbol NOTIFY symbolChanged)
   Q_PROPERTY(qreal amount MEMBER m_amount NOTIFY amountChanged)
   Q_PROPERTY(qreal yield MEMBER m_yield NOTIFY yieldChanged)
   Q_PROPERTY(qreal total READ total NOTIFY yieldChanged NOTIFY amountChanged STORED false)

   QString m_symbol;
   qreal m_amount;
   qreal m_yield;

public:
   Balance(QString symbol, qreal amount, qreal yield = 0, QObject* parent = nullptr)
      : QObject(parent),
        m_symbol(symbol),
        m_amount(amount),
        m_yield(yield)
   {}
   Balance(QString symbol, std::pair<qreal,qreal> amounts, QObject* parent = nullptr)
      : QObject(parent),
        m_symbol(symbol),
        m_amount(amounts.first),
        m_yield(amounts.second)
   {}
   virtual ~Balance(){}

   qreal total() const { return m_amount + m_yield; }
Q_SIGNALS:
   void symbolChanged(qreal arg);
   void amountChanged(qreal arg);
   void yieldChanged(qreal arg);
};
