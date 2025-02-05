/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HtmlFile.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfuente- <mfuente-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:09:48 by mfuente-          #+#    #+#             */
/*   Updated: 2025/02/04 15:38:45 by mfuente-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTMLFILE_H
#define HTMLFILE_H

#include <string>
#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>

class HtmlFile {
private:
    std::string content;
    long size;
    void readFile(std::string& filePath);
public:
    HtmlFile();
    HtmlFile(std::string filePath);
    HtmlFile(const HtmlFile &copy);
    ~HtmlFile();
    HtmlFile &operator=(const HtmlFile& otro);
    //geter
    std::string getContent() const;
    long getSize() const;
};

#endif