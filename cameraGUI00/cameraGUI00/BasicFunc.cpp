#include "BasicFunc.h"
void BasicFunc::getSubdirs(string matchstr, string path, vector<string>& files)
{
	intptr_t hFile = 0;
	struct _finddata_t fileinfo;
	std::string p;
	regex match(matchstr);
	vector<string> find_files;
	string  pathFrom =regex_replace(path, regex("\\\\+|/+"), "");

	if ((hFile = _findfirst(p.assign(pathFrom).append("\\*.*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//限制查找目标为文件夹
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					find_files.push_back(fileinfo.name);
				}
			}
			else
			{
				;
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
	for (int i = 0; i < find_files.size(); i++)
	{
		if (regex_match(find_files[i], match))
		{
			files.push_back(find_files[i]);
		}
	}
}
void BasicFunc::LoadPicArrayDate(const string & fileslocate, const string & FilenamesBob, vector<Mat>& dstImagedateArray, vector<string>& namelist, int flag)
{
	cout << fileslocate << FilenamesBob << " \n读取图片序列开始" << endl;
	int image_count = 0;
	string locate;
	locate += fileslocate;
	locate += FilenamesBob;
	ifstream fin(locate);
	string lineText;
	Size image_size;
	int erroCode = -1;
	while (getline(fin, lineText))
	{
		string filespath = fileslocate;
		smatch fileName, information;

		if (regex_search(lineText, fileName, regex("^\\w\\w*\\.[a-zA-Z]{3}")))
		{
			if (regex_search(lineText, information, regex("[/|\\\\]{2}\\w+")))
			{

				cout << "图片" << fileName[0] << "未加载,原因：" << information[0] << endl;
			}
			else
			{
				filespath += fileName[0];
				image_count++;
				Mat imageInput;

				if (flag == 0)
				{
					imageInput = imread(filespath, 0);

				}
				else
				{
					imageInput = imread(filespath);
					if (imageInput.channels() != 3)
					{
						erroCode = 0;
						throw(erroCode);
					}
				}
				dstImagedateArray.push_back(imageInput);
				namelist.push_back(fileName[0]);
				if (image_count == 1)
				{
					image_size.width = imageInput.cols;
					image_size.height = imageInput.rows;
					cout << "图片序列分辨率为：" << image_size << endl;
					cout << "图片像素格式为：" << Type2String(imageInput.type()) << endl;
				}
			}
		}
		else
		{
			continue;
		}
	}
	cout << "读取图片" << dstImagedateArray.size() << "张，读取完成！" << endl;
}

string BasicFunc::Type2String(int type)
{
	string strType;
	uchar depth = type & CV_MAT_DEPTH_MASK;
	uchar chans = 1 + (type >> CV_CN_SHIFT);
	switch (depth)
	{
	case CV_8U:
		strType = "CV_8U"; break;
	case CV_8S:
		strType = "CV_8S"; break;
	case CV_16U:
		strType = "CV_16U"; break;
	case CV_16S:
		strType = "CV_16S"; break;
	case CV_32S:
		strType = "CV_32S"; break;
	case CV_32F:
		strType = "CV_32F"; break;
	case CV_64F:
		strType = "CV_64F"; break;
	default:
		strType = "UNKNOWN_TYPE"; break;
	}
	strType += "C";
	strType += (chans + '0');

	return strType;
}
