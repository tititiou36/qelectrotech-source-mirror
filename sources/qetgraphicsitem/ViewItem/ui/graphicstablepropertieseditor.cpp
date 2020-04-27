/*
	Copyright 2006-2020 The QElectroTech Team
	This file is part of QElectroTech.

	QElectroTech is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.

	QElectroTech is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with QElectroTech.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "graphicstablepropertieseditor.h"
#include "ui_graphicstablepropertieseditor.h"
#include "qetgraphicstableitem.h"
#include "qetgraphicsheaderitem.h"
#include "diagram.h"
#include "QPropertyUndoCommand/qpropertyundocommand.h"
#include "itemmodelcommand.h"
#include "propertieseditorfactory.h"
#include "elementprovider.h"

#include <QAbstractItemModel>
#include <QFontDialog>

/**
 * @brief GraphicsTablePropertiesEditor::GraphicsTablePropertiesEditor
 * @param table
 * @param parent
 */
GraphicsTablePropertiesEditor::GraphicsTablePropertiesEditor(QetGraphicsTableItem *table, QWidget *parent) :
	PropertiesEditorWidget(parent),
    ui(new Ui::GraphicsTablePropertiesEditor)
{
    ui->setupUi(this);
	m_header_button_group = new QButtonGroup(this);
	m_header_button_group->addButton(ui->m_header_align_left_rb,   Qt::AlignLeft);
	m_header_button_group->addButton(ui->m_header_align_center_rb, Qt::AlignHCenter);
	m_header_button_group->addButton(ui->m_header_align_right_rb,  Qt::AlignRight);

	m_table_button_group = new QButtonGroup(this);
	m_table_button_group->addButton(ui->m_table_align_left_rb,   Qt::AlignLeft);
	m_table_button_group->addButton(ui->m_table_align_center_rb, Qt::AlignHCenter);
	m_table_button_group->addButton(ui->m_table_align_right_rb,  Qt::AlignRight);

	if (table) {
		setTable(table);
	}
}

/**
 * @brief GraphicsTablePropertiesEditor::~GraphicsTablePropertiesEditor
 */
GraphicsTablePropertiesEditor::~GraphicsTablePropertiesEditor() {
	delete ui;
}

/**
 * @brief GraphicsTablePropertiesEditor::setTable
 * Set the table to be edited
 * @param table
 */
void GraphicsTablePropertiesEditor::setTable(QetGraphicsTableItem *table)
{
	if (m_table_item) {
		for (auto c : m_connect_list) {
			disconnect(c);
		}
		if (m_current_model_editor)
		{
			ui->m_content_layout->removeWidget(m_current_model_editor);
			m_current_model_editor->deleteLater();
			m_current_model_editor = nullptr;
		}
	}

	m_table_item = table;
	m_connect_list.clear();
	m_connect_list << connect(m_table_item.data(), &QetGraphicsTableItem::xChanged, this, &GraphicsTablePropertiesEditor::updateUi);
	m_connect_list << connect(m_table_item.data(), &QetGraphicsTableItem::yChanged, this, &GraphicsTablePropertiesEditor::updateUi);


	if (auto editor = PropertiesEditorFactory::propertiesEditor(table->model(), this))
	{
		ui->m_content_layout->insertWidget(0, editor);
		m_current_model_editor = editor;
	}
	updateUi();
}

/**
 * @brief GraphicsTablePropertiesEditor::apply
 * Apply the current edition
 */
void GraphicsTablePropertiesEditor::apply()
{
	if(!m_table_item && m_table_item->diagram()) {
		return;
	}
	auto d = m_table_item->diagram();
	auto undo = associatedUndo();
	if (undo) {
		d->undoStack().push(undo);
	}
}

/**
 * @brief GraphicsTablePropertiesEditor::associatedUndo
 * @return the undo command associated to the edition
 */
QUndoCommand *GraphicsTablePropertiesEditor::associatedUndo() const
{
	if (m_live_edit)
	{
		if (!qFuzzyCompare(ui->m_x_pos->value(), m_table_item->pos().x())) {
			auto undo = new QPropertyUndoCommand(m_table_item.data(), "x", m_table_item->pos().x(), ui->m_x_pos->value());
			undo->setAnimated(true, false);
			undo->setText(tr("Déplacer un tableau"));
			return undo;
		}

		if (!qFuzzyCompare(ui->m_y_pos->value(), m_table_item->pos().y())) {
			auto undo = new QPropertyUndoCommand(m_table_item.data(), "y", m_table_item->pos().y(), ui->m_y_pos->value());
			undo->setAnimated(true, false);
			undo->setText(tr("Déplacer un tableau"));
			return undo;
		}

		if (ui->m_display_n_row_sb->value() != m_table_item->displayNRow()) {
			auto undo = new QPropertyUndoCommand(m_table_item.data(), "displayNRow", m_table_item->displayNRow(), ui->m_display_n_row_sb->value());
			undo->setText(tr("Modifier le nombre de ligne affiché par un tableau"));
			return undo;
		}

		QMargins header_margins(ui->m_header_left_margin->value(),
							   ui->m_header_top_margin->value(),
							   ui->m_header_right_margin->value(),
							   ui->m_header_bottom_margin->value());
		if (header_margins != m_table_item->headerItem()->margins())
		{
			QVariant old_; old_.setValue(m_table_item->headerItem()->margins());
			QVariant new_; new_.setValue(header_margins);
			auto undo = new QPropertyUndoCommand(m_table_item->headerItem(), "margins", old_, new_);
			undo->setText(tr("Modifier les marges d'une en tête de tableau"));
			return undo;
		}

		QMargins table_margins(ui->m_table_left_margin->value(),
							   ui->m_table_top_margin->value(),
							   ui->m_table_right_margin->value(),
							   ui->m_table_bottom_margin->value());
		if (table_margins != m_table_item->margins())
		{
			QVariant old_; old_.setValue(m_table_item->margins());
			QVariant new_; new_.setValue(table_margins);
			auto undo = new QPropertyUndoCommand(m_table_item.data(), "margins", old_, new_);
			undo->setText(tr("Modifier les marges d'un tableau"));
			return undo;
		}

		if (m_header_button_group->checkedId() != m_table_item->model()->headerData(0, Qt::Horizontal, Qt::TextAlignmentRole).toInt())
		{
			auto undo = new ModelHeaderDataCommand(m_table_item->model());
			undo->setData(0, Qt::Horizontal, m_header_button_group->checkedId(), Qt::TextAlignmentRole);
			undo->setText(tr("Modifier l'alignement d'une en tête de tableau"));
			return undo;
		}

		if (m_table_button_group->checkedId() != m_table_item->model()->index(0,0).data(Qt::TextAlignmentRole).toInt())
		{
			auto undo = new ModelIndexCommand(m_table_item->model(), m_table_item->model()->index(0,0));
			undo->setData(m_table_button_group->checkedId(), Qt::TextAlignmentRole);
			undo->setText(tr("Modifier l'alignement des textes d'un tableau"));
			return undo;
		}
	}

	return nullptr;
}

bool GraphicsTablePropertiesEditor::setLiveEdit(bool live_edit)
{
	if (m_live_edit == live_edit) {
		return true;
	}

	m_live_edit = live_edit;
	setUpEditConnection();
	return true;
}

/**
 * @brief GraphicsTablePropertiesEditor::on_m_header_font_pb_clicked
 */
void GraphicsTablePropertiesEditor::on_m_header_font_pb_clicked()
{
	if (m_table_item && m_table_item->model())
	{
		bool ok;
		auto font = QFontDialog::getFont(&ok,
										 m_table_item->model()->headerData(0, Qt::Horizontal, Qt::FontRole).value<QFont>(),
										 this);
		if (ok && m_table_item->model())
		{
			auto undo = new ModelHeaderDataCommand(m_table_item->model());
			undo->setData(0, Qt::Horizontal, QVariant::fromValue(font), Qt::FontRole);
			undo->setText(tr("Modifier la police d'une en tête de tableau"));
			m_table_item->diagram()->undoStack().push(undo);
		}
	}
}

/**
 * @brief GraphicsTablePropertiesEditor::on_m_table_font_pb_clicked
 */
void GraphicsTablePropertiesEditor::on_m_table_font_pb_clicked()
{
	if (m_table_item && m_table_item->model())
	{
		bool ok;
		auto index = m_table_item->model()->index(0,0);
		auto old_font = m_table_item->model()->data(index, Qt::FontRole).value<QFont>();
		auto new_font = QFontDialog::getFont(&ok, old_font, this);

		if (ok && m_table_item->diagram())
		{
			auto undo = new ModelIndexCommand(m_table_item->model(), index);
			undo->setData(QVariant::fromValue(new_font), Qt::FontRole);
			undo->setText(tr("Changer la police d'un tableau"));
			m_table_item->diagram()->undoStack().push(undo);
		}
	}
}

/**
 * @brief GraphicsTablePropertiesEditor::updateUi
 */
void GraphicsTablePropertiesEditor::updateUi()
{
		//Disconnect every connections of editor widgets
		//to avoid an unwanted edition (QSpinBox emit valueChanged no matter if changer by user or by program)
	for (QMetaObject::Connection c : m_edit_connection) {
		disconnect(c);
	}
	m_edit_connection.clear();

	ui->m_table_name_le->setText(m_table_item->tableName());
	ui->m_x_pos->setValue(m_table_item->pos().x());
	ui->m_y_pos->setValue(m_table_item->pos().y());
	ui->m_display_n_row_sb->setValue(m_table_item->displayNRow());

	ui->m_previous_table_cb->clear();
	m_other_table_vector.clear();

	ui->m_previous_table_cb->addItem(tr("Aucun")); //Add no previous table

	if (auto item_ = m_table_item->previousTable()) //Add the current previous table
	{
		m_other_table_vector.append(item_);
		ui->m_previous_table_cb->addItem(item_->tableName(), m_other_table_vector.indexOf(item_));
		ui->m_previous_table_cb->setCurrentIndex(ui->m_previous_table_cb->findData(m_other_table_vector.indexOf(item_)));
	}

	ElementProvider ep(m_table_item->diagram()->project());
	for (auto item_ : ep.table(m_table_item, m_table_item->model())) //Add available tables
	{
		if (item_ != m_table_item &&
			item_->nextTable() == nullptr)
		{
			m_other_table_vector.append(item_);
			ui->m_previous_table_cb->addItem(item_->tableName(), m_other_table_vector.indexOf(item_));
		}
	}

	auto margin = m_table_item->headerItem()->margins();
	ui->m_header_top_margin   ->setValue(margin.top());
	ui->m_header_left_margin  ->setValue(margin.left());
	ui->m_header_right_margin ->setValue(margin.right());
	ui->m_header_bottom_margin->setValue(margin.bottom());

	margin = m_table_item->margins();
	ui->m_table_top_margin   ->setValue(margin.top());
	ui->m_table_left_margin  ->setValue(margin.left());
	ui->m_table_right_margin ->setValue(margin.right());
	ui->m_table_bottom_margin->setValue(margin.bottom());


	if (!m_table_item->model()) {
		return;
	}

	if (auto button = m_header_button_group->button(m_table_item->model()->headerData(0, Qt::Horizontal, Qt::TextAlignmentRole).toInt()))
		button->setChecked(true);
	if (auto button = m_table_button_group->button(m_table_item->model()->data(m_table_item->model()->index(0,0), Qt::TextAlignmentRole).toInt()))
		button->setChecked(true);

	setUpEditConnection();
}

/**
 * @brief GraphicsTablePropertiesEditor::setUpEditConnection
 */
void GraphicsTablePropertiesEditor::setUpEditConnection()
{
	for (QMetaObject::Connection c : m_edit_connection) {
		disconnect(c);
	}

	m_edit_connection.clear();

	if (m_live_edit)
	{
		m_edit_connection << connect(ui->m_x_pos,                QOverload<int>::of(&QSpinBox::valueChanged),      this, &GraphicsTablePropertiesEditor::apply);
		m_edit_connection << connect(ui->m_y_pos,                QOverload<int>::of(&QSpinBox::valueChanged),      this, &GraphicsTablePropertiesEditor::apply);
		m_edit_connection << connect(ui->m_header_top_margin,    QOverload<int>::of(&QSpinBox::valueChanged),      this, &GraphicsTablePropertiesEditor::apply);
		m_edit_connection << connect(ui->m_header_left_margin,   QOverload<int>::of(&QSpinBox::valueChanged),      this, &GraphicsTablePropertiesEditor::apply);
		m_edit_connection << connect(ui->m_header_right_margin,  QOverload<int>::of(&QSpinBox::valueChanged),      this, &GraphicsTablePropertiesEditor::apply);
		m_edit_connection << connect(ui->m_header_bottom_margin, QOverload<int>::of(&QSpinBox::valueChanged),      this, &GraphicsTablePropertiesEditor::apply);
		m_edit_connection << connect(ui->m_table_top_margin,     QOverload<int>::of(&QSpinBox::valueChanged),      this, &GraphicsTablePropertiesEditor::apply);
		m_edit_connection << connect(ui->m_table_left_margin,    QOverload<int>::of(&QSpinBox::valueChanged),      this, &GraphicsTablePropertiesEditor::apply);
		m_edit_connection << connect(ui->m_table_right_margin,   QOverload<int>::of(&QSpinBox::valueChanged),      this, &GraphicsTablePropertiesEditor::apply);
		m_edit_connection << connect(ui->m_table_bottom_margin,  QOverload<int>::of(&QSpinBox::valueChanged),      this, &GraphicsTablePropertiesEditor::apply);
		m_edit_connection << connect(m_table_button_group,       QOverload<int>::of(&QButtonGroup::buttonClicked), this, &GraphicsTablePropertiesEditor::apply);
		m_edit_connection << connect(m_header_button_group,      QOverload<int>::of(&QButtonGroup::buttonClicked), this, &GraphicsTablePropertiesEditor::apply);
		m_edit_connection << connect(ui->m_display_n_row_sb,     QOverload<int>::of(&QSpinBox::valueChanged),      this, &GraphicsTablePropertiesEditor::apply);
	}
}

void GraphicsTablePropertiesEditor::on_m_table_name_le_textEdited(const QString &arg1) {
	m_table_item->setTableName(arg1);
}

void GraphicsTablePropertiesEditor::on_m_previous_table_cb_activated(int index)
{
	if (index == 0) {
		m_table_item->setPreviousTable();
	} else {
		m_table_item->setPreviousTable(m_other_table_vector.at(ui->m_previous_table_cb->currentData().toInt()));
	}
}
