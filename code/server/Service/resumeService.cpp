/**
 * @file resumeService.cpp
 * @brief 简历服务实现文件，处理简历管理相关业务逻辑
 * @author SDU-YueQiu
 * @date 2025/5/14
 * @version 1.0
 */

#include "resumeService.h"
#include "../Database/databaseManager.h"
#include <crow/json.h>

namespace Service {
    ResumeService::CreateResult ResumeService::createResume(
        const std::string& userId, 
        const crow::json::rvalue& resumeData) {
        
        auto& db = DAL::getDatabaseManager();
        auto query = db.prepareStatement(
            "INSERT INTO Resumes (UserID, Title, Content, Skills, Experience, Education) "
            "VALUES (?, ?, ?, ?, ?, ?)");
        query.bind(1, userId);
        query.bind(2, resumeData["title"].s());
        query.bind(3, resumeData["content"].s());
        query.bind(4, resumeData["skills"].s());
        query.bind(5, resumeData["experience"].s());
        query.bind(6, resumeData["education"].s());
        
        if (!query.exec()) {
            return {false, "Resume creation failed", ""};
        }
        
        std::string resumeId = std::to_string(db.getLastInsertRowid());
        return {true, "Resume created successfully", resumeId};
    }
    
    ResumeService::GetResult ResumeService::getResumeById(const std::string& resumeId) {
        auto& db = DAL::getDatabaseManager();
        auto query = db.prepareStatement(
            "SELECT ResumeID, UserID, Title, Content, Skills, Experience, Education "
            "FROM Resumes WHERE ResumeID = ?");
        query.bind(1, resumeId);
        
        if (!query.executeStep()) {
            return {false, "Resume not found", {}};
        }
        
        crow::json::wvalue resume;
        resume["resumeId"] = query.getColumn(0).getString();
        resume["userId"] = query.getColumn(1).getString();
        resume["title"] = query.getColumn(2).getString();
        resume["content"] = query.getColumn(3).getString();
        resume["skills"] = query.getColumn(4).getString();
        resume["experience"] = query.getColumn(5).getString();
        resume["education"] = query.getColumn(6).getString();
        
        return {true, "", resume};
    }
    
    ResumeService::ListResult ResumeService::getResumesByUserId(const std::string& userId) {
        auto& db = DAL::getDatabaseManager();
        auto query = db.prepareStatement(
            "SELECT ResumeID, Title FROM Resumes WHERE UserID = ?");
        query.bind(1, userId);
        
        crow::json::wvalue resumes;
        int index = 0;
        while (query.executeStep()) {
            crow::json::wvalue resume;
            resume["resumeId"] = query.getColumn(0).getString();
            resume["title"] = query.getColumn(1).getString();
            resumes[index++] = std::move(resume);
        }
        
        return {true, "", resumes};
    }
    
    ResumeService::UpdateResult ResumeService::updateResume(
        const std::string& resumeId, 
        const crow::json::rvalue& resumeData) {
        
        auto& db = DAL::getDatabaseManager();
        auto query = db.prepareStatement(
            "UPDATE Resumes SET Title = ?, Content = ?, Skills = ?, Experience = ?, Education = ? "
            "WHERE ResumeID = ?");
        query.bind(1, resumeData["title"].s());
        query.bind(2, resumeData["content"].s());
        query.bind(3, resumeData["skills"].s());
        query.bind(4, resumeData["experience"].s());
        query.bind(5, resumeData["education"].s());
        query.bind(6, resumeId);
        
        if (!query.exec()) {
            return {false, "Resume update failed"};
        }
        
        return {true, "Resume updated successfully"};
    }
    
    ResumeService::DeleteResult ResumeService::deleteResume(const std::string& resumeId) {
        auto& db = DAL::getDatabaseManager();
        auto query = db.prepareStatement(
            "DELETE FROM Resumes WHERE ResumeID = ?");
        query.bind(1, resumeId);
        
        if (!query.exec()) {
            return {false, "Resume deletion failed"};
        }
        
        return {true, "Resume deleted successfully"};
    }
}