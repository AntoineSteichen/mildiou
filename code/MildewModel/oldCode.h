/* BIN FOR OLD CODE IN CASE OF REUSE */





/*



void Model::fillData(std::string t_fileName, std::string h_fileName) {
    int tmpRank = rank;
    _hourly_temperatures =fillVector(std::move(t_fileName), tmpRank);
    _hourly_humidities =fillVector(std::move(h_fileName), tmpRank);
}


std::string Model::getDateFromLine(std::string line){
  int i =0;
  int tmp = 0;

    while(tmp != 3)
	     {
	       if(line[i] == ';')
		 tmp++;
	       i++;
	     }
	     return line.substr(0,i) ;
}



int Model::getHourData(std::vector<std::string> vector, std::string hour)
{
  int result = -1; 
  for(int i = 0; i < vector.size(); i++)
    {
      if( vector[i].substr(0,8) == hour)
	{
	  result = i;
	  break;
	}
    }
  return result;
}


void Model::filterDatas(std::string t_fileName, std::string h_fileName)
{ // fill both vectors for the next day
  std::vector<std::string> h_fullDay = getFullDay(h_fileName);
  std::vector<std::string> t_fullDay = getFullDay(t_fileName);
  std::vector<std::string> t_filteredDay;
  std::vector<std::string> h_filteredDay; 
  std::string tmpHour;
  std::string delimiter = ";";
  int pos = 0;

  if(t_fullDay.size() > h_fullDay.size())
    {
      // Compare t times to h times
      for(int i = 0; i< t_fullDay.size(); i++)
	{
	  tmpHour = t_fullDay[i].substr(0,8);
	  pos = getHourData(h_fullDay, tmpHour);
	    if(pos != -1)  // if exists then push both values
	      {
		t_filteredDay.push_back(t_fullDay[i]);
		h_filteredDay.push_back(h_fullDay[pos]);
	      }
	} 
    }
  else
    {
      for(int i = 0; i< h_fullDay.size(); i++)
	{
	  tmpHour = h_fullDay[i].substr(0,8);
	  pos = getHourData(t_fullDay, tmpHour);
	    if(pos != -1)  // if exists then push both values
	      {
		t_filteredDay.push_back(t_fullDay[pos]);
		h_filteredDay.push_back(h_fullDay[i]);
	      }
	} 
    }
  fillVectors(t_filteredDay, h_filteredDay);
  
}


void Model::fillVectors(std::vector<std::string> t_datas, std::vector<std::string> h_datas)
{
  double sum = 0;
    std::string delimiter = ";";
  int i = 0;
  int j = 0;
  std::string tmpHour;
  std::string tmpNextHour;
  std::string tmpData;
       while(i < t_datas.size()) //CALCUL
        {
	   tmpHour =  t_datas[i].substr(0,  t_datas[i].find(delimiter));
	   tmpNextHour =  t_datas[j].substr(0,  t_datas[j].find(delimiter));
	   tmpData =  t_datas[j].substr( t_datas[j].find(delimiter)+1,  t_datas[j].size());
	  //	  std::cout << "DATE:" <<  tmpDateVector[i] << std::endl;
	   if( tmpHour.substr(0, tmpHour.find(":")) ==tmpNextHour.substr(0, tmpNextHour.find(":")) && j <=  t_datas.size()-2)
            {
	      //      std::cout << "data: " <<std::stod(tmpDateVector[j].substr(9,2)) << std::endl;
	      sum +=std::stod(tmpData);
                j++;
            }
            else  if(j ==  t_datas.size()-1) //DERNIERE DATA
            {
	      //	        std::cout << "data: " <<std::stod(tmpDateVector[j].substr(9,2)) << std::endl;
                sum = sum/(j-i);
                _hourly_temperatures.push_back(sum);
                i = j;
                break;
            }
            else // CHANGEMENT D'HEURE
            {
                sum = sum/(j-i);
                 _hourly_temperatures.push_back(sum);
                i = j;
                sum = 0;
            }
        }
       
        while(i < h_datas.size()) //CALCUL
        {
	   tmpHour =  h_datas[i].substr(0,  h_datas[i].find(delimiter));
	   tmpNextHour =  h_datas[j].substr(0,  h_datas[j].find(delimiter));
	   tmpData =  h_datas[j].substr( h_datas[j].find(delimiter)+1,  h_datas[j].size());
	  //	  std::cout << "DATE:" <<  tmpDateVector[i] << std::endl;
	   if( tmpHour.substr(0, tmpHour.find(":")) ==tmpNextHour.substr(0, tmpNextHour.find(":")) && j <=  h_datas.size()-2)
            {
	      //      std::cout << "data: " <<std::stod(tmpDateVector[j].substr(9,2)) << std::endl;
	      sum +=std::stod(tmpData);
                j++;
            }
            else  if(j ==  h_datas.size()-1) //DERNIERE DATA
            {
	      //	        std::cout << "data: " <<std::stod(tmpDateVector[j].substr(9,2)) << std::endl;
                sum = sum/(j-i);
                _hourly_humidities.push_back(sum);
                i = j;
                break;
            }
            else // CHANGEMENT D'HEURE
            {
                sum = sum/(j-i);
                 _hourly_humidities.push_back(sum);
                i = j;
                sum = 0;
            }
        }
}






std::vector<std::string> Model::getFullDay(std::string fileName){
  std::ifstream fichier(fileName, std::ios::in);
  std::string myString;
  std::vector<std::string> dataVector;
  
  if(fichier)
    {
      getline(fichier, myString);
      if(currentDate == "") // First Day
	{
	  // getline(fichier, myString);
	  currentDate = getDateFromLine(myString); // Recupere la date
	}
      while(getDateFromLine(myString) != currentDate)
	{
	  getline(fichier, myString); // Revenir au début de la journée
	}
      // Extract all data with date = currentDate
      while(getDateFromLine(myString) == currentDate)
	{
	  getline(fichier, myString);
	  dataVector.push_back(myString.substr(currentDate.size()));	
	}
      currentDate = getDateFromLine(myString);
      return dataVector; 
      // return data vector
      // currentDate = nextDate
    }
  
}


double Model::get_sporul() {
    double result = 0;

    for(Cohorte i : _cohortes)
        result += i.sporul();

    return result;
}



std::vector<double> Model::fillVector(std::string fileName, int previousDayRank) {

    std::string myString;
    std::string currentDate;
    std::string tmpDate;
    std::string tmpHour;
    std::string tmpNextHour;
    std::string tmpData;
    std::string delimiter = ";";
    int dateSize = 0; 
    std::ifstream fichier(fileName, std::ios::in);
    int tmp_rank = 1;
    unsigned int i = 0;
    int tmp = 0;
    unsigned int j = 0;
    double sum = 0;
    std::vector<std::string> tmpDateVector;
    std::vector<double> tmpDateVector2;
    if(fichier)
    {
      while(tmp_rank !=previousDayRank+1) //RETOURNER A LA FIN DE LA JOURNEE PRECEDENTE
        {
            getline(fichier, myString);
            tmp_rank++;
        }
        while(getline(fichier, myString)) // ISOLER UNE JOURNEE
        {
           if(tmp_rank ==previousDayRank+1)
           {
	     while(tmp != 3)
	     {
	       if(myString[i] == ';')
		 tmp++;
	       i++;
	     }
	     dateSize = i;
	     i = 0;
	 
	       currentDate = myString.substr(0,dateSize) ;
	       //    std::cout << "Current Date: " << currentDate << std::endl;
               tmpDateVector.push_back(myString.substr(currentDate.size()));		 
           }
  
           else if(tmp_rank >previousDayRank && myString.substr(0,currentDate.size()) == currentDate )
           {
        
	     //  std::cout << "Current Date1: " << currentDate << std::endl;
               tmpDateVector.push_back(myString.substr(currentDate.size()));		 
	       
	    
           }
           else if(myString.substr(0,currentDate.size()) != currentDate)
               break;
            tmp_rank++;
        }

        rank = tmp_rank-1;
        fichier.close();
		std::cout << "rank" << rank << std::endl;
	/*
	     for(std::string i : tmpDateVector)
            std:: cout << i << " " ;
	     std::cout << "------------------------------------------------------------------------------------" << std::endl;
	*/   
	     	/*
        std::cout << "Vector size:" << tmpDateVector.size() << std::endl;
	*/
	/*
	for(int k = 0; k<tmpDateVector.size(); k++)
	  {
	    std::cout << "tmpdatevector" <<  tmpDateVector[k] << std::endl;
	    tmpHour = tmpDateVector[k].substr(0, tmpDateVector[k].find(delimiter));
	    tmpData = tmpDateVector[k].substr(tmpDateVector[k].find(delimiter)+1, tmpDateVector[k].size());
	    std::cout << "tmphour" <<  tmpHour << std::endl;
	    std::cout << "tmphoursubstr" <<  tmpHour.substr(0, tmpHour.find(":")) << std::endl;
	    std::cout << "tmpdata" <<  tmpData << std::endl;
	  	
	    }
 
        while(i < tmpDateVector.size()) //CALCUL
        {

	   tmpHour = tmpDateVector[i].substr(0, tmpDateVector[i].find(delimiter));
	   tmpNextHour = tmpDateVector[j].substr(0, tmpDateVector[j].find(delimiter));
	   tmpData = tmpDateVector[j].substr(tmpDateVector[j].find(delimiter)+1, tmpDateVector[j].size());
	  //	  std::cout << "DATE:" <<  tmpDateVector[i] << std::endl;
	   if( tmpHour.substr(0, tmpHour.find(":")) ==tmpNextHour.substr(0, tmpNextHour.find(":")) && j <= tmpDateVector.size()-2)
            {
	      //      std::cout << "data: " <<std::stod(tmpDateVector[j].substr(9,2)) << std::endl;
	      sum +=std::stod(tmpData);
                j++;
            }
            else  if(j == tmpDateVector.size()-1) //DERNIERE DATA
            {
	      //	        std::cout << "data: " <<std::stod(tmpDateVector[j].substr(9,2)) << std::endl;
                sum = sum/(j-i);
                tmpDateVector2.push_back(sum);
                i = j;
                break;
            }
            else // CHANGEMENT D'HEURE
            {
                sum = sum/(j-i);
                tmpDateVector2.push_back(sum);
                i = j;
                sum = 0;
            }
        }
        return tmpDateVector2;
       
    }
    else
      {
	 std::cout << "Impossible d'ouvrir le fichier Temperature!" << std::endl;
	 exit(-1);
      }
       


	    */













 
