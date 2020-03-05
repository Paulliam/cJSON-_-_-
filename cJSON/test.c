/*
  Copyright (c) 2009 Dave Gamble
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

/* Parse text to JSON, then render back to text, and print! */
void doit(char *text)
{
	char *out;cJSON *json;
	
	json=cJSON_Parse(text);
	if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
	else
	{
		out=cJSON_Print(json);
		cJSON_Delete(json);
		printf("%s\n",out);
		free(out);
	}
}

/* Read a file, parse, render back, etc. */
void dofile(char *filename)
{
	FILE *f;long len;char *data;
	
	f=fopen(filename,"rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
	data=(char*)malloc(len+1);fread(data,1,len,f);fclose(f);
	doit(data);
	free(data);
}

/* Used by some code below as an example datatype. */
struct record {const char *precision;double lat,lon;const char *address,*city,*state,*zip,*country; };

/* Create a bunch of objects as demonstration. */
void create_objects()
{
	cJSON *root,*fmt,*img,*thm,*fld;char *out;int i;	/* declare a few. */
	/* Our "days of the week" array: */
	const char *strings[7]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
	/* Our matrix: */
	int numbers[3][3]={{0,-1,0},{1,0,0},{0,0,1}};
	/* Our "gallery" item: */
	int ids[4]={116,943,234,38793};
	/* Our array of "records": */
	struct record fields[2]={
		{"zip",37.7668,-1.223959e+2,"","SAN FRANCISCO","CA","94107","US"},
		{"zip",37.371991,-1.22026e+2,"","SUNNYVALE","CA","94085","US"}};

	/* Here we construct some JSON standards, from the JSON site. */

#if 0
{
        "name": "Jack (\"Bee\") Nimble",
        "format":       {
                "type": "rect",
                "width":        1920,
                "height":       1080,
                "interlace":    false,
                "frame rate":   24
        }
}
#endif
	/* Our "Video" datatype: */
	root=cJSON_CreateObject();	//创建cJSON结构体指针变量---创建花括号
	cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble")); //增加item 异形字符串
	cJSON_AddItemToObject(root, "format", fmt=cJSON_CreateObject());//增加item 子节点
	cJSON_AddStringToObject(fmt,"type",		"rect");//子节点fmt下增加并设置键"type"的值为常规字符串"rect"
	cJSON_AddNumberToObject(fmt,"width",		1920); //子节点fmt下增加并设置键"width"的值为1920
	cJSON_AddNumberToObject(fmt,"height",		1080);
	cJSON_AddFalseToObject (fmt,"interlace"); //子节点fmt下增加并设置键"interlace"的值为false
	cJSON_AddNumberToObject(fmt,"frame rate",	24);
	
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);	/* Print to text, Delete the cJSON, print it, release the string. */


#if 0
	["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"]
#endif
	/* Our "days of the week" array: */
	root=cJSON_CreateStringArray(strings,7); //创建字符串类型的cJSON数组

	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);


#if 0
[[0, -1, 0], [1, 0, 0], [0, 0, 1]]
#endif
	/* Our matrix: */ 
	root=cJSON_CreateArray(); //创建抽象数组root
	for (i=0;i<3;i++) cJSON_AddItemToArray(root,cJSON_CreateIntArray(numbers[i],3));//增加抽象元素(即形参里创建的整形数组)到整型数组root

/*	cJSON_ReplaceItemInArray(root,1,cJSON_CreateString("Replacement")); */
	
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);


#if 0
{
        "Image":        {
                "Width":        800,
                "Height":       600,
                "Title":        "View from 15th Floor",
                "Thumbnail":    {
                        "Url":  "http:/*www.example.com/image/481989943",
                        "Height":       125,
                        "Width":        "100"
                },
                "IDs":  [116, 943, 234, 38793]
        }
}
#endif
	/* Our "gallery" item: */
	root=cJSON_CreateObject();
	cJSON_AddItemToObject(root, "Image", img=cJSON_CreateObject());
	cJSON_AddNumberToObject(img,"Width",800);
	cJSON_AddNumberToObject(img,"Height",600);
	cJSON_AddStringToObject(img,"Title","View from 15th Floor");
	cJSON_AddItemToObject(img, "Thumbnail", thm=cJSON_CreateObject());
	cJSON_AddStringToObject(thm, "Url", "http:/*www.example.com/image/481989943");
	cJSON_AddNumberToObject(thm,"Height",125);
	cJSON_AddStringToObject(thm,"Width","100");
	cJSON_AddItemToObject(img,"IDs", cJSON_CreateIntArray(ids,4));	//元素类型比较特殊时，调用cJSON_AddItemToObject()

	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);


#if 0
[{
                "precision":    "zip",
                "Latitude":     37.766800,
                "Longitude":    -122.395900,
                "Address":      "",
                "City": "SAN FRANCISCO",
                "State":        "CA",
                "Zip":  "94107",
                "Country":      "US"
        }, {
                "precision":    "zip",
                "Latitude":     37.371991,
                "Longitude":    -122.026000,
                "Address":      "",
                "City": "SUNNYVALE",
                "State":        "CA",
                "Zip":  "94085",
                "Country":      "US"
        }]
#endif
	/* Our array of "records": */
	root=cJSON_CreateArray();
	for (i=0;i<2;i++)
	{
		cJSON_AddItemToArray(root,fld=cJSON_CreateObject());
		cJSON_AddStringToObject(fld, "precision", fields[i].precision);
		cJSON_AddNumberToObject(fld, "Latitude", fields[i].lat);
		cJSON_AddNumberToObject(fld, "Longitude", fields[i].lon);
		cJSON_AddStringToObject(fld, "Address", fields[i].address);
		cJSON_AddStringToObject(fld, "City", fields[i].city);
		cJSON_AddStringToObject(fld, "State", fields[i].state);
		cJSON_AddStringToObject(fld, "Zip", fields[i].zip);
		cJSON_AddStringToObject(fld, "Country", fields[i].country);
	}
	
/*	cJSON_ReplaceItemInObject(cJSON_GetArrayItem(root,1),"City",cJSON_CreateIntArray(ids,4)); */
	
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);	free(out);

}

int main (int argc, const char * argv[]) {
	/* a bunch of json: */
	char text1[]="{\n\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n\"format\": {\"type\":       \"rect\", \n\"width\":      1920, \n\"height\":     1080, \n\"interlace\":  false,\"frame rate\": 24\n}\n}";	
	char text2[]="[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
	char text3[]="[\n    [0, -1, 0],\n    [1, 0, 0],\n    [0, 0, 1]\n	]\n";
	char text4[]="{\n		\"Image\": {\n			\"Width\":  800,\n			\"Height\": 600,\n			\"Title\":  \"View from 15th Floor\",\n			\"Thumbnail\": {\n				\"Url\":    \"http:/*www.example.com/image/481989943\",\n				\"Height\": 125,\n				\"Width\":  \"100\"\n			},\n			\"IDs\": [116, 943, 234, 38793]\n		}\n	}";
	char text5[]="[\n	 {\n	 \"precision\": \"zip\",\n	 \"Latitude\":  37.7668,\n	 \"Longitude\": -122.3959,\n	 \"Address\":   \"\",\n	 \"City\":      \"SAN FRANCISCO\",\n	 \"State\":     \"CA\",\n	 \"Zip\":       \"94107\",\n	 \"Country\":   \"US\"\n	 },\n	 {\n	 \"precision\": \"zip\",\n	 \"Latitude\":  37.371991,\n	 \"Longitude\": -122.026020,\n	 \"Address\":   \"\",\n	 \"City\":      \"SUNNYVALE\",\n	 \"State\":     \"CA\",\n	 \"Zip\":       \"94085\",\n	 \"Country\":   \"US\"\n	 }\n	 ]";

	/* Process each json textblock by parsing, then rebuilding: */
	doit(text1);
	doit(text2);	
	doit(text3);
	doit(text4);
	doit(text5);

	/* Parse standard testfiles: */
/*	dofile("../../tests/test1"); */
/*	dofile("../../tests/test2"); */
/*	dofile("../../tests/test3"); */
/*	dofile("../../tests/test4"); */
/*	dofile("../../tests/test5"); */

	/* Now some samplecode for building objects concisely: */
	create_objects();
	
	return 0;
}
