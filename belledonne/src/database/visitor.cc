/// @file visitor.cc
/// @brief Visitor passed to Libclang - implementation
/// @author gm
/// @copyright gm 2016
///
/// This file is part of Belledonne
///
/// Belledonne is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// Belledonne is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with Belledonne.  If not, see <http://www.gnu.org/licenses/>.

#include "belledonne/src/database/visitor.h"

#include "belledonne/src/database/db_manager.h"

namespace belledonne {
namespace database {

EverythingIndexer::EverythingIndexer(const std::string& translation_unit,
                                     DBManager& data_manager)
    : translation_unit_(translation_unit),
      data_manager_(data_manager) {
}

CXChildVisitResult EverythingIndexer::visit(CXCursor cursor, CXCursor /*parent*/) {
  const CXSourceLocation source_location(clang_getCursorLocation(cursor));
  CXFile file;
  unsigned int line;
  unsigned int column;
  unsigned int offset;
  clang_getExpansionLocation(source_location,
                             &file, &line, &column, &offset);
  const CXString filename = clang_getFileName(file);
  const char* cursor_filename = clang_getCString(filename);
  // Skip includes
  if (!filename.data
      || strcmp(cursor_filename, translation_unit_.c_str()) != 0 ) {
    clang_disposeString(filename);
    return CXChildVisit_Continue;
  }

  const CXCursorKind kind = clang_getCursorKind(cursor);
  const std::string cursor_usr(clang_getCString(clang_getCursorUSR(cursor)));
  const CXCursor ref_cursor(clang_getCursorReferenced(cursor));
  const std::string ref_cursor_usr(clang_getCString(clang_getCursorUSR(ref_cursor)));

  const CXSourceLocation ref_source_location(clang_getCursorLocation(ref_cursor));
  CXFile ref_file;
  unsigned int ref_line;
  unsigned int ref_column;
  unsigned int ref_offset;
  clang_getExpansionLocation(ref_source_location,
                             &ref_file, &ref_line, &ref_column, &ref_offset);

  if (!cursor_usr.empty()) {
    const SymbolType type(GetSymbolType(kind));
    BELLEDONNE_ASSERT(type != SymbolType::kUnknown);
    if (type != SymbolType::kIgnored) {
      const CXString cursor_name(clang_getCursorDisplayName(cursor));
      const DatabaseEntry entry(std::string(clang_getCString(cursor_name)), std::string(cursor_filename), type, line, column);
      data_manager_.Put(cursor_usr, entry);
      clang_disposeString(cursor_name);
    }
  }
  clang_disposeString(filename);
  return CXChildVisit_Recurse;

  //const CXCursor refCursor = clang_getCursorReferenced(cursor);
  //if (!clang_equalCursors(refCursor, clang_getNullCursor())) {
  //  CXFile ref_file;
  //  unsigned int ref_line;
  //  unsigned int ref_column;
  //  unsigned int ref_offset;
  //  clang_getExpansionLocation(clang_getCursorLocation(refCursor),
  //                             &ref_file, &ref_line, &ref_column, &ref_offset);

  //  CXString ref_filename = clang_getFileName(ref_file);
  //  if (ref_filename.data) {
  //    const std::string cursor_usr(clang_getCString(clang_getCursorUSR(refCursor)));
  //    if (!cursor_usr.empty()) {
  //      const SymbolType type(GetSymbolType(kind));
  //      BELLEDONNE_ASSERT(type != SymbolType::kUnknown);
  //      //if (type != SymbolType::kIgnored) {
  //        const DatabaseEntry entry(std::string(cursor_filename), type, line, column);
  //        dictionary_[cursor_usr] = entry;
  //      //}
  //    }
  //  }
  //  clang_disposeString(ref_filename);
  //}
  //clang_disposeString(filename);
  //return CXChildVisit_Recurse;
}

CXChildVisitResult EverythingIndexer::visitorFunction(
  CXCursor cursor,
  CXCursor parent,
  CXClientData clientData) {
  EverythingIndexer* visitor = static_cast<EverythingIndexer*>(clientData);
  return visitor->visit(cursor, parent);
}

SymbolType EverythingIndexer::GetSymbolType(CXCursorKind cursor_kind) const {
  // The following map is lean and mean and purpose: it makes maintenance a lot easier
  switch (cursor_kind) {
    case CXCursor_UnexposedDecl:
      return SymbolType::kIgnored;
    case CXCursor_StructDecl:
      return SymbolType::kStruct;
    case CXCursor_UnionDecl:
      return SymbolType::kStruct;
    case CXCursor_ClassDecl:
      return SymbolType::kStruct;
    case CXCursor_EnumDecl:
      return SymbolType::kStruct;
    case CXCursor_FieldDecl:
      return SymbolType::kField;
    case CXCursor_EnumConstantDecl:
      return SymbolType::kIgnored;
    case CXCursor_FunctionDecl:
      return SymbolType::kFunction;
    case CXCursor_VarDecl:
      return SymbolType::kVar;
    case CXCursor_ParmDecl:
      return SymbolType::kVar;
    case CXCursor_ObjCInterfaceDecl:
    case CXCursor_ObjCCategoryDecl:
    case CXCursor_ObjCProtocolDecl:
    case CXCursor_ObjCPropertyDecl:
    case CXCursor_ObjCIvarDecl:
    case CXCursor_ObjCInstanceMethodDecl:
    case CXCursor_ObjCClassMethodDecl:
    case CXCursor_ObjCImplementationDecl:
    case CXCursor_ObjCCategoryImplDecl:
    case CXCursor_TypedefDecl:
      return SymbolType::kIgnored;
    case CXCursor_CXXMethod:
      return SymbolType::kFunction;
    case CXCursor_Namespace:
      return SymbolType::kIgnored;
    case CXCursor_LinkageSpec:
      return SymbolType::kIgnored;
    case CXCursor_Constructor:
      return SymbolType::kFunction;
    case CXCursor_Destructor:
      return SymbolType::kFunction;
    case CXCursor_ConversionFunction:
      return SymbolType::kFunction;
    case CXCursor_TemplateTypeParameter:
      return SymbolType::kIgnored;
    case CXCursor_NonTypeTemplateParameter:
      return SymbolType::kIgnored;
    case CXCursor_TemplateTemplateParameter:
      return SymbolType::kIgnored;
    case CXCursor_FunctionTemplate:
      return SymbolType::kIgnored;
    case CXCursor_ClassTemplate:
      return SymbolType::kIgnored;
    case CXCursor_ClassTemplatePartialSpecialization:
      return SymbolType::kIgnored;
    case CXCursor_NamespaceAlias:
      return SymbolType::kIgnored;
    case CXCursor_UsingDirective:
      return SymbolType::kIgnored;
    case CXCursor_UsingDeclaration:
      return SymbolType::kIgnored;
    case CXCursor_TypeAliasDecl:
      return SymbolType::kIgnored;
    case CXCursor_ObjCSynthesizeDecl:
    case CXCursor_ObjCDynamicDecl:
      return SymbolType::kIgnored;
    case CXCursor_CXXAccessSpecifier:
      return SymbolType::kIgnored;
   // case CXCursor_FirstDecl:
      //return SymbolType::kIgnored;
   // case CXCursor_LastDecl:
      //return SymbolType::kIgnored;
    case CXCursor_FirstRef:
      return SymbolType::kIgnored;
   // case CXCursor_ObjCSuperClassRef:
      //return SymbolType::kIgnored;
    case CXCursor_ObjCClassRef:
      return SymbolType::kIgnored;
    case CXCursor_TypeRef:
      return SymbolType::kIgnored;
    case CXCursor_CXXBaseSpecifier:
      return SymbolType::kIgnored;
    case CXCursor_TemplateRef:
      return SymbolType::kIgnored;
    case CXCursor_NamespaceRef:
      return SymbolType::kIgnored;
    case CXCursor_MemberRef:
      return SymbolType::kIgnored;
    case CXCursor_LabelRef:
      return SymbolType::kIgnored;
    case CXCursor_OverloadedDeclRef:
      return SymbolType::kIgnored;
    case CXCursor_VariableRef:
      return SymbolType::kIgnored;
   // case CXCursor_LastRef:
      //return SymbolType::kIgnored;
    case CXCursor_FirstInvalid:
      return SymbolType::kIgnored;
   // case CXCursor_InvalidFile:
      //return SymbolType::kIgnored;
    case CXCursor_NoDeclFound:
      return SymbolType::kIgnored;
    case CXCursor_NotImplemented:
      return SymbolType::kIgnored;
    case CXCursor_InvalidCode:
      return SymbolType::kIgnored;
   // case CXCursor_LastInvalid:
      //return SymbolType::kIgnored;
    case CXCursor_FirstExpr:
      return SymbolType::kIgnored;
   // case CXCursor_UnexposedExpr:
      //return SymbolType::kIgnored;
    case CXCursor_DeclRefExpr:
      return SymbolType::kIgnored;
    case CXCursor_MemberRefExpr:
      return SymbolType::kIgnored;
    case CXCursor_CallExpr:
      return SymbolType::kIgnored;
    case CXCursor_ObjCMessageExpr:
      return SymbolType::kIgnored;
    case CXCursor_BlockExpr:
      return SymbolType::kIgnored;
    case CXCursor_IntegerLiteral:
      return SymbolType::kIgnored;
    case CXCursor_FloatingLiteral:
      return SymbolType::kIgnored;
    case CXCursor_ImaginaryLiteral:
      return SymbolType::kIgnored;
    case CXCursor_StringLiteral:
      return SymbolType::kIgnored;
    case CXCursor_CharacterLiteral:
      return SymbolType::kIgnored;
    case CXCursor_ParenExpr:
      return SymbolType::kIgnored;
    case CXCursor_UnaryOperator:
      return SymbolType::kIgnored;
    case CXCursor_ArraySubscriptExpr:
      return SymbolType::kIgnored;
    case CXCursor_BinaryOperator:
      return SymbolType::kIgnored;
    case CXCursor_CompoundAssignOperator:
      return SymbolType::kIgnored;
    case CXCursor_ConditionalOperator:
      return SymbolType::kIgnored;
    case CXCursor_CStyleCastExpr:
      return SymbolType::kIgnored;
    case CXCursor_CompoundLiteralExpr:
      return SymbolType::kIgnored;
    case CXCursor_InitListExpr:
      return SymbolType::kIgnored;
    case CXCursor_AddrLabelExpr:
      return SymbolType::kIgnored;
    case CXCursor_StmtExpr:
      return SymbolType::kIgnored;
    case CXCursor_GenericSelectionExpr:
      return SymbolType::kIgnored;
    case CXCursor_GNUNullExpr:
      return SymbolType::kIgnored;
    case CXCursor_CXXStaticCastExpr:
      return SymbolType::kIgnored;
    case CXCursor_CXXDynamicCastExpr:
      return SymbolType::kIgnored;
    case CXCursor_CXXReinterpretCastExpr:
      return SymbolType::kIgnored;
    case CXCursor_CXXConstCastExpr:
      return SymbolType::kIgnored;
    case CXCursor_CXXFunctionalCastExpr:
      return SymbolType::kIgnored;
    case CXCursor_CXXTypeidExpr:
      return SymbolType::kIgnored;
    case CXCursor_CXXBoolLiteralExpr:
      return SymbolType::kIgnored;
    case CXCursor_CXXNullPtrLiteralExpr:
      return SymbolType::kIgnored;
    case CXCursor_CXXThisExpr:
      return SymbolType::kIgnored;
    case CXCursor_CXXThrowExpr:
      return SymbolType::kIgnored;
    case CXCursor_CXXNewExpr:
      return SymbolType::kIgnored;
    case CXCursor_CXXDeleteExpr:
      return SymbolType::kIgnored;
    case CXCursor_UnaryExpr:
      return SymbolType::kIgnored;
    case CXCursor_ObjCStringLiteral:
    case CXCursor_ObjCEncodeExpr:;
    case CXCursor_ObjCSelectorExpr:
    case CXCursor_ObjCProtocolExpr:
    case CXCursor_ObjCBridgedCastExpr:
      return SymbolType::kIgnored;
    case CXCursor_PackExpansionExpr:
      return SymbolType::kIgnored;
    case CXCursor_SizeOfPackExpr:
      return SymbolType::kIgnored;
    case CXCursor_LambdaExpr:
      return SymbolType::kIgnored;
    case CXCursor_ObjCBoolLiteralExpr:
    case CXCursor_ObjCSelfExpr:
      return SymbolType::kIgnored;
   // case CXCursor_LastExpr:
      //return SymbolType::kIgnored;
    case CXCursor_FirstStmt:
      return SymbolType::kIgnored;
   // case CXCursor_UnexposedStmt:
      //return SymbolType::kIgnored;
    case CXCursor_LabelStmt:
      return SymbolType::kIgnored;
    case CXCursor_CompoundStmt:
      return SymbolType::kIgnored;
    case CXCursor_CaseStmt:
      return SymbolType::kIgnored;
    case CXCursor_DefaultStmt:
      return SymbolType::kIgnored;
    case CXCursor_IfStmt:
      return SymbolType::kIgnored;
    case CXCursor_SwitchStmt:
      return SymbolType::kIgnored;
    case CXCursor_WhileStmt:
      return SymbolType::kIgnored;
    case CXCursor_DoStmt:
      return SymbolType::kIgnored;
    case CXCursor_ForStmt:
      return SymbolType::kIgnored;
    case CXCursor_GotoStmt:
      return SymbolType::kIgnored;
    case CXCursor_IndirectGotoStmt:
      return SymbolType::kIgnored;
    case CXCursor_ContinueStmt:
      return SymbolType::kIgnored;
    case CXCursor_BreakStmt:
      return SymbolType::kIgnored;
    case CXCursor_ReturnStmt:
      return SymbolType::kIgnored;
    case CXCursor_GCCAsmStmt:
      return SymbolType::kIgnored;
   // case CXCursor_AsmStmt:
      //return SymbolType::kIgnored;
    case CXCursor_ObjCAtTryStmt:
    case CXCursor_ObjCAtCatchStmt:
    case CXCursor_ObjCAtFinallyStmt:
    case CXCursor_ObjCAtThrowStmt:
    case CXCursor_ObjCAtSynchronizedStmt:
    case CXCursor_ObjCAutoreleasePoolStmt:
    case CXCursor_ObjCForCollectionStmt:
      return SymbolType::kIgnored;
    case CXCursor_CXXCatchStmt:
      return SymbolType::kIgnored;
    case CXCursor_CXXTryStmt:
      return SymbolType::kIgnored;
    case CXCursor_CXXForRangeStmt:
      return SymbolType::kIgnored;
    case CXCursor_SEHTryStmt:
      return SymbolType::kIgnored;
    case CXCursor_SEHExceptStmt:
      return SymbolType::kIgnored;
    case CXCursor_SEHFinallyStmt:
      return SymbolType::kIgnored;
    case CXCursor_MSAsmStmt:
      return SymbolType::kIgnored;
    case CXCursor_NullStmt:
      return SymbolType::kIgnored;
    case CXCursor_DeclStmt:
      return SymbolType::kIgnored;
    case CXCursor_OMPParallelDirective:
    case CXCursor_OMPSimdDirective:
    case CXCursor_OMPForDirective:
    case CXCursor_OMPSectionsDirective:
    case CXCursor_OMPSectionDirective:
    case CXCursor_OMPSingleDirective:
    case CXCursor_OMPParallelForDirective:
    case CXCursor_OMPParallelSectionsDirective:
    case CXCursor_OMPTaskDirective:
    case CXCursor_OMPMasterDirective:
    case CXCursor_OMPCriticalDirective:
    case CXCursor_OMPTaskyieldDirective:
    case CXCursor_OMPBarrierDirective:
    case CXCursor_OMPTaskwaitDirective:
    case CXCursor_OMPFlushDirective:
      return SymbolType::kIgnored;
    case CXCursor_SEHLeaveStmt:
      return SymbolType::kIgnored;
    case CXCursor_OMPOrderedDirective:
    case CXCursor_OMPAtomicDirective:
    case CXCursor_OMPForSimdDirective:
    case CXCursor_OMPParallelForSimdDirective:
    case CXCursor_OMPTargetDirective:
    case CXCursor_OMPTeamsDirective:
    case CXCursor_OMPTaskgroupDirective:
    case CXCursor_OMPCancellationPointDirective:
    case CXCursor_OMPCancelDirective:
      return SymbolType::kIgnored;
   // case CXCursor_LastStmt:
      //return SymbolType::kIgnored;
    case CXCursor_TranslationUnit:
      return SymbolType::kIgnored;
    case CXCursor_FirstAttr:
      return SymbolType::kIgnored;
   // case CXCursor_UnexposedAttr:
      //return SymbolType::kIgnored;
    case CXCursor_IBActionAttr:
      return SymbolType::kIgnored;
    case CXCursor_IBOutletAttr:
      return SymbolType::kIgnored;
    case CXCursor_IBOutletCollectionAttr:
      return SymbolType::kIgnored;
    case CXCursor_CXXFinalAttr:
      return SymbolType::kIgnored;
    case CXCursor_CXXOverrideAttr:
      return SymbolType::kIgnored;
    case CXCursor_AnnotateAttr:
      return SymbolType::kIgnored;
    case CXCursor_AsmLabelAttr:
      return SymbolType::kIgnored;
    case CXCursor_PackedAttr:
      return SymbolType::kIgnored;
    case CXCursor_PureAttr:
      return SymbolType::kIgnored;
    case CXCursor_ConstAttr:
      return SymbolType::kIgnored;
    case CXCursor_NoDuplicateAttr:
      return SymbolType::kIgnored;
    case CXCursor_CUDAConstantAttr:
    case CXCursor_CUDADeviceAttr:
    case CXCursor_CUDAGlobalAttr:
    case CXCursor_CUDAHostAttr:
    case CXCursor_CUDASharedAttr:
      return SymbolType::kIgnored;
   // case CXCursor_LastAttr:
      //return SymbolType::kIgnored;
    case CXCursor_PreprocessingDirective:
      return SymbolType::kIgnored;
    case CXCursor_MacroDefinition:
      return SymbolType::kIgnored;
    case CXCursor_MacroExpansion:
      return SymbolType::kIgnored;
   // case CXCursor_MacroInstantiation:
      //return SymbolType::kIgnored;
    case CXCursor_InclusionDirective:
      return SymbolType::kIgnored;
   // case CXCursor_FirstPreprocessing:
      //return SymbolType::kIgnored;
   // case CXCursor_LastPreprocessing:
      //return SymbolType::kIgnored;
    case CXCursor_ModuleImportDecl:
      return SymbolType::kIgnored;
   // case CXCursor_FirstExtraDecl:
      //return SymbolType::kIgnored;
   // case CXCursor_LastExtraDecl:
   //   return SymbolType::kIgnored;
    case CXCursor_OverloadCandidate:
      return SymbolType::kIgnored;
    default:
      return SymbolType::kUnknown;
  }
}

};  // namespace database
};  // namespace belledonne
