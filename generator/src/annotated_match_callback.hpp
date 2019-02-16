/*
** Created by doom on 13/02/19.
*/

#ifndef ECHOGEN_ANNOTATED_MATCH_CALLBACK_HPP
#define ECHOGEN_ANNOTATED_MATCH_CALLBACK_HPP

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cctype>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>

namespace echogen
{
    class annotated_field
    {
    private:
        const clang::CXXRecordDecl *record_;
        const clang::FieldDecl *field_;

    public:
        annotated_field(const clang::CXXRecordDecl *record, const clang::FieldDecl *field) noexcept :
            record_(record), field_(field)
        {
        }

        void dump_field_descriptor(llvm::raw_ostream &os) const noexcept
        {
            os << "            {\n";
            os << "                get_type<";
            field_->getType().print(os, field_->getASTContext().getPrintingPolicy());
            os << ">(),\n";
            os << "                \"" << field_->getName() << "\",\n";
            os << "                offsetof(" << record_->getName() << ", " << field_->getName() << "),\n";
            os << "            },\n";
        }
    };

    class annotated_function
    {
    private:
        const clang::CXXRecordDecl *record_;
        const clang::FunctionDecl *function_;

    public:
        annotated_function(const clang::CXXRecordDecl *record, const clang::FunctionDecl *function) noexcept :
            record_(record), function_(function)
        {
        }

        void dump_parameters_descriptors(llvm::raw_ostream &os) const noexcept
        {
            os << "        static const field_descriptor " << function_->getName() << "_params[] = {\n";
            for (auto param_it = function_->param_begin(); param_it != function_->param_end(); ++param_it) {
                os << "            {\n";
                os << "                get_type<";
                (*param_it)->getType().print(os, (*param_it)->getASTContext().getPrintingPolicy());
                os << ">(),\n";
                os << "                \"" << (*param_it)->getName() << "\",\n";
                os << "                0,\n";
                os << "            },\n";
            }
            os << "        };\n";
        }

        void dump_function_descriptor(llvm::raw_ostream &os) const noexcept
        {
            os << "            {\n";
            os << "                get_type<";
            function_->getReturnType().print(os, function_->getASTContext().getPrintingPolicy());
            os << ">(),\n";
            os << "                \"" << function_->getName() << "\",\n";
            os << "                {" << function_->getName() << "_params, sizeof("
               << function_->getName() << "_params) / sizeof("
               << function_->getName() << "_params[0])},\n";
            os << "            },\n";
        }
    };

    class annotated_class
    {
    private:
        const clang::CXXRecordDecl *record_;
        std::vector<annotated_field> fields_;
        std::vector<annotated_function> functions_;

    public:
        explicit annotated_class(const clang::CXXRecordDecl *record) noexcept : record_(record)
        {
        }

        void add_annotated_field(const clang::FieldDecl *field) noexcept
        {
            fields_.emplace_back(record_, field);
        }

        void add_annotated_function(const clang::FunctionDecl *function) noexcept
        {
            functions_.emplace_back(record_, function);
        }

        void dump_reflection_data(llvm::raw_ostream &os) const noexcept
        {
            os << "namespace echo\n";
            os << "{\n";
            os << "    template <>\n";
            os << "    inline const class_descriptor &get_class<" << record_->getName() << ">() noexcept\n";
            os << "    {\n";

            if (not fields_.empty()) {
                os << "        static const field_descriptor fields[] = {\n";
                for (const auto &f : fields_) {
                    f.dump_field_descriptor(os);
                }
                os << "        };\n";
            }

            if (not functions_.empty()) {
                for (const auto &f : functions_) {
                    f.dump_parameters_descriptors(os);
                }
                os << "        static const function_descriptor functions[] = {\n";
                for (const auto &f : functions_) {
                    f.dump_function_descriptor(os);
                }
                os << "        };\n";
            }

            os << "        static const class_descriptor desc{\n";
            os << "            type_descriptor{\n";
            os << "                \"" << record_->getName() << "\",\n";
            os << "                sizeof(" << record_->getName() << "),\n";
            os << "                type_tags::class_tag,\n";
            os << "                details::hash(\"" << record_->getName() << "\"),\n";
            os << "            },\n";
            if (not fields_.empty()) {
                os << "            {fields, sizeof(fields) / sizeof(fields[0])},\n";
            } else {
                os << "            {},\n";
            }
            if (not functions_.empty()) {
                os << "            {functions, sizeof(functions) / sizeof(functions[0])},\n";
            } else {
                os << "            {},\n";
            }
            os << "        };\n";
            os << "\n";
            os << "        return desc;\n";
            os << "    }\n";
            os << "\n";

            os << "    template <>\n";
            os << "    inline const type_descriptor &get_type<" << record_->getName() << ">() noexcept\n";
            os << "    {\n";
            os << "        return get_class<" << record_->getName() << ">();\n";
            os << "    }\n";

            os << "}\n\n";
        }
    };

    class annotated_match_callback : public clang::ast_matchers::MatchFinder::MatchCallback
    {
    private:
        std::optional<std::filesystem::path> out_path_;
        std::vector<annotated_class> classes_;

        void handle_record_decl(const clang::CXXRecordDecl *record) noexcept
        {
            classes_.emplace_back(record);
        }

        void handle_field_decl(const clang::FieldDecl *field)
        {
            assert(not classes_.empty());
            classes_.back().add_annotated_field(field);
        }

        void handle_function_decl(const clang::FunctionDecl *function)
        {
            assert(not classes_.empty());
            classes_.back().add_annotated_function(function);
        }

        void set_file_path(const clang::Decl *decl, const clang::SourceManager *source_manager) noexcept
        {
            if (not out_path_) {
                auto loc = decl->getSourceRange().getBegin();
                auto name = source_manager->getFilename(loc);
                std::filesystem::path path = (std::string)name;
                path.replace_filename(path.filename().stem().string() + ".echo" + path.extension().string());
                out_path_ = path;
            }
        }

    public:
        void run(const clang::ast_matchers::MatchFinder::MatchResult &result) override
        {
            if (auto record = result.Nodes.getNodeAs<clang::CXXRecordDecl>("id"); record) {
                set_file_path(record, result.SourceManager);
                handle_record_decl(record);
            } else if (auto field = result.Nodes.getNodeAs<clang::FieldDecl>("id"); field) {
                handle_field_decl(field);
            } else if (auto function = result.Nodes.getNodeAs<clang::FunctionDecl>("id"); function) {
                handle_function_decl(function);
            }
        }

        void onEndOfTranslationUnit() override
        {
            if (not out_path_) {
                llvm::errs() << "no echo'ed structure or class was found\n";
                return;
            }
            llvm::outs() << "generating echo'ed data for " << *out_path_ << "\n";

            std::error_code ec;
            llvm::raw_fd_ostream ofs((*out_path_).string(), ec);
            assert(!ec);

            std::string guard_macro = out_path_->filename().string();
            std::transform(guard_macro.begin(), guard_macro.end(), guard_macro.begin(), [](auto &&c) {
                return std::isalnum(c) ? std::toupper(c) : '_';
            });

            ofs << "#ifndef " << guard_macro << "\n";
            ofs << "#define " << guard_macro << "\n\n";

            for (const auto &cls : classes_) {
                cls.dump_reflection_data(ofs);
            }

            ofs << "#endif /* !" << guard_macro << " */\n";

            out_path_.reset();
            classes_.clear();
        }
    };
}

#endif /* !ECHOGEN_ANNOTATED_MATCH_CALLBACK_HPP */
