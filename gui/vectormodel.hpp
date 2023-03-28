#pragma once

#include <vector>

#include <QAbstractTableModel>
#include <QStringList>

template <class Data> class VectorModel : public QAbstractTableModel
{
public:
    using Container = std::vector<Data>;

    explicit VectorModel(const QStringList& header_labels, QObject* parent = nullptr)
        : QAbstractTableModel{parent}
        , m_header_labels{header_labels}
    {
    }

    ~VectorModel() override = default;

    int rowCount(const QModelIndex& index = {}) const override
    {
        return m_data.size();
    }

    int columnCount(const QModelIndex& index = {}) const override
    {
        return m_header_labels.size();
    }

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override
    {
        if(index.isValid() && index.row() >= 0 && index.row() < rowCount() && index.column() >= 0
           && index.column() < columnCount() && role == Qt::DisplayRole)
        {
            return dataFromEntry(m_data[index.row()], index.column());
        }
        else
        {
            return QVariant();
        }
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {
        if(orientation == Qt::Horizontal && section >= 0 && section < m_header_labels.size() && role == Qt::DisplayRole)
            return m_header_labels[section];
        else
            return QAbstractTableModel::headerData(section, orientation, role);
    }

    void updateData(const Container& data)
    {
        beginResetModel();
        m_data = data;
        endResetModel();
    }

    virtual QVariant dataFromEntry(const Data& data, const int column_index) const = 0;

private:
    QStringList m_header_labels;
    Container m_data;
};