//#pragma once
//
//#include <QString>
//#include <QStringView>
//
///**
// * @namespace tactile::prefs::id
// *
// * @brief Provides functions for obtaining keys for various settings.
// *
// * @since 0.1.0
// */
//namespace tactile::prefs::id {
//
//namespace graphics {
//
///**
// * @brief Returns the key associated with whether or not a grid is rendered
// * over tilemaps.
// *
// * @details The associated value is a `bool`.
// *
// * @return the render grid setting identifier.
// *
// * @since 0.1.0
// */
//[[nodiscard]] inline auto grid() -> const QString&
//{
//  static const QString str{"graphics/grid"};
//  return str;
//}
//
//// represents a QPalette
////[[nodiscard]] inline auto theme() -> const QString&
////{
////  static const QString str{"graphics/theme"};
////  return str;
////}
//
//// represents a QString
//[[nodiscard]] inline auto theme_name() -> const QString&
//{
//  static const QString str{"graphics/themeName"};
//  return str;
//}
//
//}  // namespace graphics
//
////namespace window {
////
/////**
//// * @brief Returns the key associated with the last window layout geometry.
//// *
//// * @details The associated value is a `QByteArray`.
//// *
//// * @return the window layout geometry identifier.
//// *
//// * @since 0.1.0
//// */
////[[nodiscard]] inline auto last_layout_geometry() -> const QString&
////{
////  static const QString str{"window/lastLayoutGeometry"};
////  return str;
////}
////
/////**
//// * @brief Returns the key associated with the last window layout state.
//// *
//// * @details The associated value is a `QByteArray`.
//// *
//// * @return the window layout state identifier.
//// *
//// * @since 0.1.0
//// */
////[[nodiscard]] inline auto last_layout_state() -> const QString&
////{
////  static const QString str{"window/lastLayoutState"};
////  return str;
////}
////
////}  // namespace window
//
//}  // namespace tactile::prefs::id
