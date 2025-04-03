import { Inject, Injectable } from '@angular/core';
import { HttpClient } from "@angular/common/http"
import { Observable } from 'rxjs';
import { Temphum } from './temphum';
import { environment } from 'src/environments/environment';

@Injectable({
  providedIn: 'root'
})
export class TemphumService {

  private apiURL = environment.apiURL;

  constructor(private http: HttpClient) {}

  public getTempHums(): Observable<Temphum[]>{
    return this.http.get<Temphum[]>(`${this.apiURL}/get`);
  }

  public addTempHum(temphum: Temphum): Observable<Temphum> {
    return this.http.post<Temphum>(`${this.apiURL}/post`, temphum);
  }

  public getTemps(): Observable<number[]>{
    return this.http.get<number[]>(`${this.apiURL}/getTemps`);
  } 

  public getHums(): Observable<number[]>{
    return this.http.get<number[]>(`${this.apiURL}/getHums`);
  } 

  public getIndexes(): Observable<number[]>{
    return this.http.get<number[]>(`${this.apiURL}/getIndexes`);
  } 
  // public getTemps2(): Observable<number[]>{
  //   return this.http.get<number[]>(`${this.apiURL}/getTemps2`);
  // } 

  // public getHums2(): Observable<number[]>{
  //   return this.http.get<number[]>(`${this.apiURL}/getHums2`);
  // } 

  // public getIndexes2(): Observable<number[]>{
  //   return this.http.get<number[]>(`${this.apiURL}/getIndexes2`);
  // } 
}
